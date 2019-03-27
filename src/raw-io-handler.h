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

#ifndef RAW_IO_HANDLER_H
#define RAW_IO_HANDLER_H

//============================================================================
//                                   INCLUDES
//============================================================================
#include <QImageIOHandler>

#include <memory>

//============================================================================
//                            FORWARD DECLARATIONS
//============================================================================
class QImage;
class QByteArray;
class QIODevice;
class QVariant;

class RawIOHandlerPrivate;

/**
 * @brief The RawIOHandler class implements the functionality of Libraw to make
 * it usable for the use with the QImageReader.
 */
class RawIOHandler : public QImageIOHandler
{
public:
    RawIOHandler();
    ~RawIOHandler() override;

    /**
     * Rule of five.
     */
    Q_DISABLE_COPY(RawIOHandler);
    RawIOHandler(const RawIOHandler&& rhs) = delete;
    RawIOHandler& operator=(const RawIOHandler&& rhs) = delete;

    /**
     * @brief Tests if the RawIOHandler can read image data from the given @a device.
     * @returns true on success, false otherwise
     */
    static bool canRead(QIODevice* device);

    // reimplemented virtual functions ----------------------------------------
    bool canRead() const override;
    bool read(QImage* image) override;
    QVariant option(ImageOption option) const override;
    void setOption(ImageOption option, const QVariant& value) override;
    bool supportsOption(ImageOption option) const override;

private:
    std::unique_ptr<RawIOHandlerPrivate> d;
};

#endif // RAW_IO_HANDLER_H
