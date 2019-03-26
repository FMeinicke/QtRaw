/*
 * Copyright (C) 2012 Alberto Mardegan <info@mardy.it>
 * Copyright (C) 2019 Florian Meinicke <florian.meinicke@t-online.de>
 *
 * This file is part of QtRaw.
 *
 * QtRaw is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QtRaw is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QtRaw.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "datastream.h"
#include "raw-io-handler.h"

#include <array>

#include <QDebug>
#include <QImage>
#include <QVariant>

#include "libraw.h"

using namespace std;

/**
 * @brief Private data of the RawIOHandler class - pimpl.
 */
class RawIOHandlerPrivate
{
public:
    RawIOHandlerPrivate(RawIOHandler* qq) :
        raw(nullptr),
        stream(nullptr),
        q(qq)
    {}

    ~RawIOHandlerPrivate() = default;

    Q_DISABLE_COPY(RawIOHandlerPrivate);
    RawIOHandlerPrivate(const RawIOHandlerPrivate&& rhs) = delete;
    RawIOHandlerPrivate& operator=(const RawIOHandlerPrivate&& rhs) = delete;

    /**
     * @brief Opens the internal @b Datastream from the given @a device.
     * @returns true on success
     * @returns false on failure
     */
    bool openDatastream(QIODevice* device);

    unique_ptr<LibRaw> raw;
    unique_ptr<Datastream> stream;
    QSize defaultSize;
    QSize scaledSize;
    mutable RawIOHandler* q;
};
//============================================================================
bool RawIOHandlerPrivate::openDatastream(QIODevice* device)
{
    if (!device)
    {
        return false;
    }

    device->seek(0);
    if (raw)
    {
        return true;
    }

    stream = make_unique<Datastream>(device);
    raw = make_unique<LibRaw>();
    if (raw->open_datastream(stream.get()) != LIBRAW_SUCCESS)
    {
        raw.reset(nullptr);
        stream.reset(nullptr);
        return false;
    }

    defaultSize = QSize(raw->imgdata.sizes.width,
                        raw->imgdata.sizes.height);
    if (raw->imgdata.sizes.flip == 5 || raw->imgdata.sizes.flip == 6)
    {
        defaultSize.transpose();
    }
    return true;
}


//============================================================================
RawIOHandler::RawIOHandler() :
    d(make_unique<RawIOHandlerPrivate>(this))
{
}

//============================================================================
RawIOHandler::~RawIOHandler() = default;

//============================================================================
bool RawIOHandler::canRead(QIODevice* device)
{
    if (!device)
    {
        return false;
    }
    RawIOHandler handler;
    return handler.d->openDatastream(device);
}

//============================================================================
bool RawIOHandler::canRead() const
{
    if (canRead(device()))
    {
        setFormat("raw");
        return true;
    }
    return false;
}

//============================================================================
bool RawIOHandler::read(QImage* image)
{
    if (!d->openDatastream(device()))
    {
        return false;
    }

    const auto finalSize = d->scaledSize.isValid() ?
                           d->scaledSize : d->defaultSize;

    const auto& imgdata = d->raw->imgdata;
    const auto deleter = [](auto* i){ LibRaw::dcraw_clear_mem(i); };
    unique_ptr<libraw_processed_image_t, decltype(deleter)> output(nullptr, deleter);
    auto ErrorCode = int{};

    if (finalSize.width() < imgdata.thumbnail.twidth ||
        finalSize.height() < imgdata.thumbnail.theight)
    {
        qDebug() << "Using thumbnail";
        d->raw->unpack_thumb();
        output.reset(d->raw->dcraw_make_mem_thumb(&ErrorCode));
    }
    else
    {
        qDebug() << "Decoding raw data";
        d->raw->unpack();
        d->raw->dcraw_process();
        output.reset(d->raw->dcraw_make_mem_image(&ErrorCode));
    }

    // Check for possible errors that occured during LibRaw loading/processing
    if (ErrorCode != LIBRAW_SUCCESS || errno != EXIT_SUCCESS)
    {
        perror("ERROR DRUING DECODING");
        qCritical("Error code: %d; LibRaw error: %d\nAborting RawIOHandler::read(QImage*)",
                  errno, ErrorCode);
        return false;
    }
    if (!output)
    {
        qCritical("Output image is a null image! Aborting RawIOHandler::read(QImage*)");
        return false;
    }

    auto unscaled = QImage{};
    if (output->type == LIBRAW_IMAGE_JPEG)
    {
        unscaled.loadFromData(output->data, output->data_size, "JPEG");
        if (imgdata.sizes.flip != 0)
        {
            int angle = 0;
            if (imgdata.sizes.flip == 3)
            {
                angle = 180;
            }
            else if (imgdata.sizes.flip == 5)
            {
                angle = -90;
            }
            else if (imgdata.sizes.flip == 6)
            {
                angle = 90;
            }
            if (angle != 0)
            {
                const auto rotation = [&angle]
                                      {
                                          auto rot = QTransform{};
                                          rot.rotate(angle);
                                          return rot;
                                      }();
                unscaled = unscaled.transformed(rotation);
            }
        }
    }
    else
    {
        const auto numPixels = output->width * output->height;
        const auto colorSize = output->bits / 8;
        const auto pixelSize = output->colors * colorSize;
        auto pixels = make_unique<uchar[]>(numPixels * 4);
        auto data = output->data;
        for (int i = 0; i < numPixels; ++i, data += pixelSize)
        {
            if (output->colors == 3)
            {
                pixels[i * 4    ] = data[2 * colorSize];
                pixels[i * 4 + 1] = data[1 * colorSize];
                pixels[i * 4 + 2] = data[0];
            }
            else
            {
                pixels[i * 4    ] = data[0];
                pixels[i * 4 + 1] = data[0];
                pixels[i * 4 + 2] = data[0];
            }
        }
        unscaled = QImage(pixels.release(),
                          output->width, output->height,
                          QImage::Format_RGB32);
    }

    if (unscaled.size() != finalSize)
    {
        // TODO: use quality parameter to decide transformation method
        *image = unscaled.scaled(finalSize, Qt::IgnoreAspectRatio,
                                 Qt::SmoothTransformation);
    }
    else
    {
        *image = unscaled;
        if (output->type == LIBRAW_IMAGE_BITMAP)
        {
            // make sure that the bits are copied
            // NOTE: better call detach() explicitly ???
            const auto* b = image->bits();
            Q_UNUSED(b);
        }
    }

    return true;
}

//============================================================================
QVariant RawIOHandler::option(ImageOption option) const
{
    switch (option)
    {
    case ImageFormat:
        return QImage::Format_RGB32;

    case Size:
        d->openDatastream(device());
        return d->defaultSize;

    case ScaledSize:
        return d->scaledSize;

    default:
        break;
    }
    return QVariant();
}

//============================================================================
void RawIOHandler::setOption(ImageOption option, const QVariant& value)
{
    switch (option)
    {
    case ScaledSize:
        d->scaledSize = value.toSize();
        break;

    default:
        break;
    }
}

//============================================================================
bool RawIOHandler::supportsOption(ImageOption option) const
{
    switch (option)
    {
    case ImageFormat:
    case Size:
    case ScaledSize:
        return true;

    default:
        break;
    }
    return false;
}
