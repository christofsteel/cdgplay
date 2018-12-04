/*
 * Copyright (c) 2013-2016 Thomas Isaac Lightburn
 *
 *
 * This file is part of OpenKJ.
 *
 * OpenKJ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "dlgcdg.h"
#include "ui_dlgcdg.h"
#include "arguments.h"
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QDir>
#include <QImageReader>

extern Arguments *arguments;

DlgCdg::DlgCdg(QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::DlgCdg)
{  
    this->parent = parent;
    hSizeAdjustment = 0;
    vSizeAdjustment = 0;
    hOffset = 0;
    vOffset = 0;
    ui->setupUi(this);
    m_fullScreen = false;
    m_lastSize.setWidth(300);
    m_lastSize.setHeight(216);
    connect(ui->cdgVideo, SIGNAL(resized(QSize)), this, SLOT(cdgSurfaceResized(QSize)));
    fullScreenTimer = new QTimer(this);
    fullScreenTimer->setInterval(500);

    cdg = new CDG;
    cdg->FileOpen(arguments->getCDG().toStdString());
    cdg->Process();
    ui->cdgVideo->videoSurface()->start();

    mediaPlayer = new QMediaPlayer;
    mediaPlayer->setMedia(QUrl::fromLocalFile(arguments->getMp3()));
    mediaPlayer->setNotifyInterval(50);
    mediaPlayer->setVolume(100);

    connect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(audioBackend_positionChanged(qint64)));
    connect(mediaPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(audioBackend_mediaStatusChanged(QMediaPlayer::MediaStatus)));

    makeFullscreen();

    mediaPlayer->play();
    show();

}

void DlgCdg::audioBackend_mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if(status == QMediaPlayer::EndOfMedia)
    {
        QApplication::quit();
    }
}

DlgCdg::~DlgCdg()
{
    delete ui;
}

void DlgCdg::updateCDG(QImage image, bool overrideVisibleCheck)
{
    if ((isVisible()) || (overrideVisibleCheck))
    {
            ui->cdgVideo->videoSurface()->present(QVideoFrame(image));
    }
}

void DlgCdg::makeFullscreen()
{
    m_fullScreen = true;
    m_lastSize.setHeight(height());
    m_lastSize.setWidth(width());
    Qt::WindowFlags flags;
    flags |= Qt::Window;
    flags |= Qt::FramelessWindowHint;
    setWindowFlags(flags);
    QRect screenDimensions = QApplication::desktop()->screenGeometry(arguments->getMonitor());
    move(screenDimensions.left()  + hOffset, screenDimensions.top() + vOffset);
    resize(screenDimensions.width() + hSizeAdjustment,screenDimensions.height() + vSizeAdjustment);
    show();
    fullScreenTimer->start();
}

void DlgCdg::makeWindowed()
{
    setWindowFlags(Qt::Window);
    resize(300, 216);
    show();
    ui->cdgVideo->repaint();
    m_fullScreen = false;
}

void DlgCdg::setFullScreen(bool fullscreen)
{
    if (fullscreen)
        makeFullscreen();
    else
        makeWindowed();
}

void DlgCdg::setFullScreenMonitor(int monitor)
{
    Q_UNUSED(monitor);    
    makeWindowed();
    makeFullscreen();
}

void DlgCdg::setVOffset(int pixels)
{
    vOffset = pixels;
    if (m_fullScreen)
    {
        QRect screenDimensions = QApplication::desktop()->screenGeometry(arguments->getMonitor());
        move(screenDimensions.left()  + hOffset, screenDimensions.top() + vOffset);
    }
}

void DlgCdg::setHOffset(int pixels)
{
    hOffset = pixels;
    if (m_fullScreen)
    {
        QRect screenDimensions = QApplication::desktop()->screenGeometry(arguments->getMonitor());
        move(screenDimensions.left()  + hOffset, screenDimensions.top() + vOffset);
    }
}

void DlgCdg::setVSizeAdjustment(int pixels)
{
    vSizeAdjustment = pixels;
    if (m_fullScreen)
    {
        QRect screenDimensions = QApplication::desktop()->screenGeometry(arguments->getMonitor());
        resize(screenDimensions.width() + hSizeAdjustment,screenDimensions.height() + vSizeAdjustment);
    }
}

void DlgCdg::setHSizeAdjustment(int pixels)
{
    hSizeAdjustment = pixels;
    if (m_fullScreen)
    {
        QRect screenDimensions = QApplication::desktop()->screenGeometry(arguments->getMonitor());
        resize(screenDimensions.width() + hSizeAdjustment,screenDimensions.height() + vSizeAdjustment);
    }
}

void DlgCdg::cdgSurfaceResized(QSize size)
{
    Q_UNUSED(size)
}



void DlgCdg::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    if (m_fullScreen)
    {
        makeWindowed();
    }
    else
        makeFullscreen();
}

void DlgCdg::fullScreenTimerTimeout()
{
    // This is to work around Windows 10 opening the window offset from the top left corner unless we wait a bit
    // before moving it.
    setVOffset(0);
    setHOffset(0);
    ui->cdgVideo->repaint();
    fullScreenTimer->stop();
}

void DlgCdg::audioBackend_positionChanged(qint64 position)
{    
        if (cdg->IsOpen() && cdg->GetLastCDGUpdate() >= position)
        {
            if (!cdg->SkipFrame(position))
            {
                unsigned char* rgbdata;
                rgbdata = cdg->GetImageByTime(position);
                QImage img(rgbdata, 300, 216, QImage::Format_RGB888);
//                ui->cdgOutput->setPixmap(QPixmap::fromImage(img));
                this->updateCDG(img);
                free(rgbdata);
            }
        }   
}

