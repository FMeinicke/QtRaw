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
/// \file   MainWindow.cpp
/// \author Florian Meinicke
/// \date   10.03.2019
/// \brief  Implementation of the MainWindow class.
//============================================================================

//============================================================================
//                                   INCLUDES
//============================================================================
#include "MainWindow.h"

#include <QAction>
#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QImageReader>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QStandardPaths>

//============================================================================
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_ImageLabel(new QLabel),
      m_ScrollArea(new QScrollArea)
{
    m_ImageLabel->setBackgroundRole(QPalette::Base);
    m_ImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_ImageLabel->setScaledContents(true);

    m_ScrollArea->setBackgroundRole(QPalette::Dark);
    m_ScrollArea->setWidget(m_ImageLabel);
    m_ScrollArea->setVisible(false);
    setCentralWidget(m_ScrollArea);

    createActions();

    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

//============================================================================
void MainWindow::createActions()
{
    auto FileMenu = menuBar()->addMenu(tr("&File"));

    auto OpenAct = FileMenu->addAction(tr("&Open..."), this, &MainWindow::open);
    OpenAct->setShortcut(QKeySequence::Open);

    FileMenu->addSeparator();

    auto ExitAct = FileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    ExitAct->setShortcut(tr("Ctrl+Q"));

    auto ViewMenu = menuBar()->addMenu(tr("&View"));

    m_ZoomInAct = ViewMenu->addAction(tr("Zoom &In (25%)"), this, &MainWindow::zoomIn);
    m_ZoomInAct->setShortcut(QKeySequence::ZoomIn);
    m_ZoomInAct->setEnabled(false);

    m_ZoomOutAct = ViewMenu->addAction(tr("Zoom &Out (25%)"), this, &MainWindow::zoomOut);
    m_ZoomOutAct->setShortcut(QKeySequence::ZoomOut);
    m_ZoomOutAct->setEnabled(false);

    m_NormalSizeAct = ViewMenu->addAction(tr("&Normal Size"), this, &MainWindow::normalSize);
    m_NormalSizeAct->setShortcut(tr("Ctrl+S"));
    m_NormalSizeAct->setEnabled(false);

    ViewMenu->addSeparator();

    m_FitToWindowAct = ViewMenu->addAction(tr("&Fit to Window"), this, &MainWindow::fitToWindow);
    m_FitToWindowAct->setEnabled(false);
    m_FitToWindowAct->setCheckable(true);
    m_FitToWindowAct->setShortcut(tr("Ctrl+F"));
}

//============================================================================
void MainWindow::updateActions()
{
    m_ZoomInAct->setEnabled(!m_FitToWindowAct->isChecked());
    m_ZoomOutAct->setEnabled(!m_FitToWindowAct->isChecked());
    m_NormalSizeAct->setEnabled(!m_FitToWindowAct->isChecked());
}

//============================================================================
static void initializeImageFileDialog(QFileDialog& Dialog)
{
    static auto FirstDialog = true;

    if (FirstDialog)
    {
        FirstDialog = false;
        const auto PicturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        Dialog.setDirectory(PicturesLocations.isEmpty() ? QDir::currentPath() : PicturesLocations.constLast());
    }

    auto MimeTypeFilters = QStringList{};
    const auto SupportedMimeTypes = QImageReader::supportedMimeTypes();
    for (const auto& MimeTypeName : SupportedMimeTypes)
    {
        MimeTypeFilters.append(MimeTypeName);
    }
    MimeTypeFilters.sort();
    Dialog.setMimeTypeFilters(MimeTypeFilters);
    Dialog.selectMimeTypeFilter("image/jpeg");
}

//============================================================================
void MainWindow::open()
{
    QFileDialog Dialog{this, tr("Open File")};
    initializeImageFileDialog(Dialog);

    while (Dialog.exec() == QDialog::Accepted &&
           !loadImage(Dialog.selectedFiles().constFirst())) {}
}

//============================================================================
bool MainWindow::loadImage(const QString& FileName)
{
    QImageReader Reader{FileName};
    Reader.setAutoTransform(true);

    const auto NewImage = Reader.read();

    if (NewImage.isNull())
    {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(FileName), Reader.errorString()));
        return false;
    }
    m_Image = NewImage;
    m_ImageLabel->setPixmap(QPixmap::fromImage(m_Image));
    m_ScaleFactor = 1.0;

    m_ScrollArea->setVisible(true);
    m_FitToWindowAct->setEnabled(true);
    updateActions();

    if (!m_FitToWindowAct->isChecked())
    {
        m_ImageLabel->adjustSize();
    }

    return true;
}

//============================================================================
void MainWindow::zoomIn()
{
    scaleImage(1.25);
}

//============================================================================
void MainWindow::zoomOut()
{
    scaleImage(0.8);
}

//============================================================================
void MainWindow::normalSize()
{
    m_ImageLabel->adjustSize();
    m_ScaleFactor = 1.0;
}

//============================================================================
void MainWindow::fitToWindow()
{
    const auto FitToWindow = m_FitToWindowAct->isChecked();
    m_ScrollArea->setWidgetResizable(FitToWindow);
    if (!FitToWindow)
    {
        normalSize();
    }
    updateActions();
}

//============================================================================
void MainWindow::scaleImage(double Factor)
{
    Q_ASSERT(m_ImageLabel->pixmap());
    m_ScaleFactor *= Factor;
    m_ImageLabel->resize(m_ScaleFactor * m_ImageLabel->pixmap()->size());

    /**
     * @brief Set the value of the given @a ScrollBar according to @b Factor.
     */
    auto adjustScrollBar = [&Factor] (QScrollBar* ScrollBar)
    {
        ScrollBar->setValue(int(Factor * ScrollBar->value()
                                + ((Factor - 1) * ScrollBar->pageStep()/2)));
    };
    //------------------------------------------------------------------------

    adjustScrollBar(m_ScrollArea->horizontalScrollBar());
    adjustScrollBar(m_ScrollArea->verticalScrollBar());

    m_ZoomInAct->setEnabled(m_ScaleFactor < 6.0);
    m_ZoomOutAct->setEnabled(m_ScaleFactor > 0.167);
}
