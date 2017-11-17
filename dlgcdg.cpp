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
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
// #include <QSvgRenderer>
#include <QPainter>
#include <QDir>
#include <QImageReader>

DlgCdg::DlgCdg(QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::DlgCdg)
{
    if (settings->cdgWindowFullScreenMonitor() > QApplication::desktop()->numScreens())
    {
        settings->setCdgWindowFullscreen(false);
    }
    hSizeAdjustment = settings->cdgHSizeAdjustment();
    vSizeAdjustment = settings->cdgVSizeAdjustment();
    hOffset = settings->cdgHOffset();
    vOffset = settings->cdgVOffset();
    ui->setupUi(this);
    m_fullScreen = false;
    m_lastSize.setWidth(300);
    m_lastSize.setHeight(216);
    connect(ui->cdgVideo, SIGNAL(resized(QSize)), this, SLOT(cdgSurfaceResized(QSize)));
    fullScreenTimer = new QTimer(this);
    fullScreenTimer->setInterval(500);

    cdg = new CDG;
    cdg->FileOpen("/home/christoph/kar.cdg");
    cdg->Process();
    ui->cdgVideo->videoSurface()->start();

    mediaPlayer = new QMediaPlayer;
    mediaPlayer->setMedia(QUrl::fromLocalFile("/home/christoph/kar.mp3"));
    mediaPlayer->setNotifyInterval(50);
    mediaPlayer->setVolume(100);

    connect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(audioBackend_positionChanged(qint64)));
    if ((settings->cdgWindowFullscreen()) && (settings->showCdgWindow()))
    {
        makeFullscreen();
    }
    mediaPlayer->play();
    show();

}

DlgCdg::~DlgCdg()
{
    delete ui;
}

void DlgCdg::updateCDG(QImage image, bool overrideVisibleCheck)
{
    if ((isVisible()) || (overrideVisibleCheck))
    {
     //   if (image.size().height() > ui->cdgVideo->size().height() || image.size().width() > ui->cdgVideo->size().width())
     //       ui->cdgVideo->videoSurface()->present(QVideoFrame(image));
     //       ui->cdgVideo->videoSurface()->present(QVideoFrame(image.scaled(ui->cdgVideo->size(), Qt::IgnoreAspectRatio)));
     //   else
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
    QRect screenDimensions = QApplication::desktop()->screenGeometry(settings->cdgWindowFullScreenMonitor());
    move(screenDimensions.left()  + hOffset, screenDimensions.top() + vOffset);
    resize(screenDimensions.width() + hSizeAdjustment,screenDimensions.height() + vSizeAdjustment);
    show();
    fullScreenTimer->start();
}

void DlgCdg::makeWindowed()
{
    setWindowFlags(Qt::Window);
    resize(300, 216);
    if (settings->showCdgWindow())
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
    if (settings->cdgWindowFullscreen())
    {
        makeWindowed();
        makeFullscreen();
    }
}

void DlgCdg::setVOffset(int pixels)
{
    vOffset = pixels;
    if (m_fullScreen)
    {
        QRect screenDimensions = QApplication::desktop()->screenGeometry(settings->cdgWindowFullScreenMonitor());
        move(screenDimensions.left()  + hOffset, screenDimensions.top() + vOffset);
    }
}

void DlgCdg::setHOffset(int pixels)
{
    hOffset = pixels;
    if (m_fullScreen)
    {
        QRect screenDimensions = QApplication::desktop()->screenGeometry(settings->cdgWindowFullScreenMonitor());
        move(screenDimensions.left()  + hOffset, screenDimensions.top() + vOffset);
    }
}

void DlgCdg::setVSizeAdjustment(int pixels)
{
    vSizeAdjustment = pixels;
    if (m_fullScreen)
    {
        QRect screenDimensions = QApplication::desktop()->screenGeometry(settings->cdgWindowFullScreenMonitor());
        resize(screenDimensions.width() + hSizeAdjustment,screenDimensions.height() + vSizeAdjustment);
    }
}

void DlgCdg::setHSizeAdjustment(int pixels)
{
    hSizeAdjustment = pixels;
    if (m_fullScreen)
    {
        QRect screenDimensions = QApplication::desktop()->screenGeometry(settings->cdgWindowFullScreenMonitor());
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
    if ((settings->showCdgWindow()) && (settings->cdgWindowFullscreen()))
    {
        setVOffset(settings->cdgVOffset());
        setHOffset(settings->cdgHOffset());
        ui->cdgVideo->repaint();
        fullScreenTimer->stop();
    }
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

