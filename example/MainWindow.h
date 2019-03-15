/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

//============================================================================
/// \file   MainWindow.h
/// \author Florian Meinicke
/// \date   10.03.2019
/// \brief  Declaration of the MainWindow class.
//============================================================================

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//============================================================================
//                                   INCLUDES
//============================================================================

#include <QImage>
#include <QMainWindow>

//============================================================================
//                            FORWARD DECLARATIONS
//============================================================================
class QLabel;
class QScrollArea;
class QScrollBar;
class QAction;

/**
 * @brief The MainWindow class displays a ScrollArea containing an Image.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Contsruct a MainWindow with @a parent as the parent widget.
     */
    MainWindow(QWidget* parent = nullptr);
    /**
     * Rule of five.
     */
    MainWindow(const MainWindow& rhs) = delete;
    MainWindow& operator=(const MainWindow& rhs) = delete;
    MainWindow(const MainWindow&& rhs) = delete;
    MainWindow& operator=(const MainWindow&& rhs) = delete;

    /**
     * @brief Destruct a MainWindow.
     */
    ~MainWindow() override = default;

    /**
     * @brief Creates all necessary actions.
     */
    void createActions();

    /**
     * @brief Updates the sizing actions based on wether the FitToWindow option
     * is currently used.
     */
    void updateActions();

    /**
     * @brief Opens up a FileDialog from where the user can choose an image to
     * display.
     */
    void open();

    /**
     * @brief Loads the image from the file specified by @a FileName into the
     * internal QImage object.
     */
    bool loadImage(const QString& FileName);

    /**
     * @brief Zoom into the image by 25%.
     */
    void zoomIn();

    /**
     * @brief Zoom out of the image by 25%.
     */
    void zoomOut();

    /**
     * @brief Set the zooming back to a factor of 1.
     */
    void normalSize();

    /**
     * @brief Adjusts width and height of the image to fit the window's dimensions.
     */
    void fitToWindow();

    /**
     * @brief Scales the internal QImage object by the given @a Factor.
     */
    void scaleImage(double Factor);

private:
    QImage m_Image;
    QLabel* m_ImageLabel{};
    QScrollArea* m_ScrollArea{};
    double m_ScaleFactor{1};

    QAction* m_ZoomInAct{};
    QAction* m_ZoomOutAct{};
    QAction* m_NormalSizeAct{};
    QAction* m_FitToWindowAct{};
};

#endif // MAINWINDOW_H
