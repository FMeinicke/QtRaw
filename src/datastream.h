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

#ifndef DATASTREAM_H
#define DATASTREAM_H

#include <QImageIOHandler>

#include "libraw_datastream.h"

class QIODevice;

/**
 * @brief The Datastream class provides an interface that makes it possible to
 * use a QIODevice as source for a LibRaw_datastream.
 */
class Datastream : public LibRaw_abstract_datastream
{
public:
    /**
     * @brief Construct a new Datastream from the given @a device.
     */
    Datastream(QIODevice* device);

    /**
     * @brief Destruct the Datastream.
     */
    ~Datastream() override = default;

    /**
     * Rule of five.
     */
    Q_DISABLE_COPY(Datastream);
    Datastream(const Datastream&& rhs) = delete;
    Datastream& operator=(const Datastream&& rhs) = delete;

    // reimplemented virtual methods -----------------------------------------
    int valid() override;
    int read(void* ptr, size_t size, size_t nmemb) override;
    int seek(INT64 offset, int whence) override;
    INT64 tell() override;
    INT64 size() override;
    int get_char() override;
    char* gets(char* s, int n) override;
    int scanf_one(const char* fmt, void* val) override;
    int eof() override;
    void* make_jas_stream() override;

private:
    QIODevice* m_device;
};

#endif // DATASTREAM_H
