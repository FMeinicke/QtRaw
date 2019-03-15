/*
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


//============================================================================
/// \file   main.cpp
/// \author Florian Meinicke
/// \date   10.03.2019
/// \brief  Implementation Declaration of the  class.
//============================================================================

//============================================================================
//                                   INCLUDES
//============================================================================
#include "MainWindow.h"
#include <QApplication>
#include <QTime>

void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QByteArray localTime = QTime::currentTime().toString().toLatin1();

    switch (type)
    {
    case QtDebugMsg:
        fprintf(stdout, "\e[1m%s: DEBUG:\e[0m %s (%s:%u, %s)\n", localTime.constData(), localMsg.constData(), context.file, context.line, context.function);
        break;

    case QtInfoMsg:
        fprintf(stdout, "\e[1m%s: INFO:\e[0m %s (%s:%u, %s)\n", localTime.constData(), localMsg.constData(), context.file, context.line, context.function);
        break;

    case QtWarningMsg:
        fprintf(stderr, "\e[1m%s: WARNING:\e[0m %s (%s:%u, %s)\n", localTime.constData(), localMsg.constData(), context.file, context.line, context.function);
        break;

    case QtCriticalMsg:
        fprintf(stderr, "\e[1m%s: CRITICAL:\e[0m %s (%s:%u, %s)\n", localTime.constData(), localMsg.constData(), context.file, context.line, context.function);
        break;

    case QtFatalMsg:
        fprintf(stderr, "\e[1m%s: FATAL:\e[0m %s (%s:%u, %s)\n", localTime.constData(), localMsg.constData(), context.file, context.line, context.function);
        abort();
        break;
    }

    fflush(stderr);
    fflush(stdout);
}


int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);

    QApplication App(argc, argv);
    MainWindow Window;
    Window.show();

    return App.exec();
}
