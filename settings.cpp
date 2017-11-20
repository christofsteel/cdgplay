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
#include "settings.h"
#include <QCoreApplication>
#include <QApplication>
#include <QDesktopWidget>
#include <QStandardPaths>
#include <QDebug>

Settings::Settings(QObject *parent) :
    QObject(parent)
{
    QCoreApplication::setOrganizationName("OpenKJ");
    QCoreApplication::setOrganizationDomain("OpenKJ.org");
    QCoreApplication::setApplicationName("OpenKJ");
    settings = new QSettings(this);
}

bool Settings::cdgWindowFullscreen()
{
    return true;
    return settings->value("cdgWindowFullscreen", false).toBool();
}

void Settings::setCdgWindowFullscreen(bool fullScreen)
{
    settings->setValue("cdgWindowFullscreen", fullScreen);
    emit cdgWindowFullscreenChanged(fullScreen);
}


bool Settings::showCdgWindow()
{
    return true;
    return settings->value("showCdgWindow", false).toBool();
}

void Settings::setShowCdgWindow(bool show)
{
    settings->setValue("showCdgWindow", show);
    emit cdgShowCdgWindowChanged(show);
}

void Settings::setCdgWindowFullscreenMonitor(int monitor)
{
    settings->setValue("cdgWindowFullScreenMonitor", monitor);
    emit cdgWindowFullscreenMonitorChanged(monitor);
}

int Settings::cdgWindowFullScreenMonitor()
{
    //We default to the highest mointor present, by default, rather than the primary display.  Seems to make more sense
    //and will help prevent people from popping up a full screen window in front of the main window and getting confused.
    return QApplication::desktop()->screenCount() - 1;
}

void Settings::saveWindowState(QWidget *window)
{
    settings->beginGroup(window->objectName());
    settings->setValue("size", window->size());
    settings->setValue("pos", window->pos());
    settings->endGroup();
}

void Settings::restoreWindowState(QWidget *window)
{

    settings->beginGroup(window->objectName());
    if (settings->contains("size") && settings->contains("pos"))
    {
        window->resize(settings->value("size", QSize(640, 480)).toSize());
        window->move(settings->value("pos", QPoint(100, 100)).toPoint());
    }
    settings->endGroup();
}

void Settings::saveColumnWidths(QTreeView *treeView)
{
    settings->beginGroup(treeView->objectName());
    settings->setValue("headerState", treeView->header()->saveState());
    settings->setValue("hiddenSections", treeView->header()->hiddenSectionCount());
    settings->setValue("sections", treeView->header()->count());
    settings->endGroup();
}

void Settings::saveColumnWidths(QTableView *tableView)
{
    settings->beginGroup(tableView->objectName());
    for (int i=0; i < tableView->horizontalHeader()->count(); i++)
    {
        settings->beginGroup(QString::number(i));
        settings->setValue("size", tableView->horizontalHeader()->sectionSize(i));
        settings->setValue("hidden", tableView->horizontalHeader()->isSectionHidden(i));
        settings->endGroup();
    }
    settings->endGroup();
}

void Settings::restoreColumnWidths(QTreeView *treeView)
{
    settings->beginGroup(treeView->objectName());
    if ((settings->contains("headerState")) && (settings->value("hiddenSections").toInt() == treeView->header()->hiddenSectionCount()) && (settings->value("sections").toInt() == treeView->header()->count()))
        treeView->header()->restoreState(settings->value("headerState").toByteArray());
    settings->endGroup();
}

void Settings::restoreColumnWidths(QTableView *tableView)
{
    settings->beginGroup(tableView->objectName());
    QStringList headers = settings->childGroups();
    for (int i=0; i < headers.size(); i++)
    {
        settings->beginGroup(headers.at(i));
        int section = headers.at(i).toInt();
        bool hidden = settings->value("hidden", false).toBool();
        int size = settings->value("size", 0).toInt();
        tableView->horizontalHeader()->resizeSection(section, size);
        tableView->horizontalHeader()->setSectionHidden(section, hidden);
        settings->endGroup();
    }
    settings->endGroup();
}

void Settings::saveSplitterState(QSplitter *splitter)
{
    settings->beginGroup(splitter->objectName());
    settings->setValue("splitterState", splitter->saveState());
    settings->endGroup();
}

void Settings::restoreSplitterState(QSplitter *splitter)
{
    settings->beginGroup(splitter->objectName());
    if (settings->contains("splitterState"))
        splitter->restoreState(settings->value("splitterState").toByteArray());
    settings->endGroup();
}

bool Settings::requestServerEnabled()
{
    return settings->value("requestServerEnabled", false).toBool();
}

void Settings::setRequestServerEnabled(bool enable)
{
    settings->setValue("requestServerEnabled", enable);
}

QString Settings::requestServerUrl()
{
    QString url = settings->value("requestServerUrl", "https://songbook.openkj.org/api").toString();
    if (url == "http://songbook.openkj.org/api") {
        url = "https://songbook.openkj.org/api";
        setRequestServerUrl(url);
    }
    return url;
}

void Settings::setRequestServerUrl(QString url)
{
    settings->setValue("requestServerUrl", url);
}

int Settings::requestServerVenue()
{
    return settings->value("requestServerVenue", 0).toInt();
}

void Settings::setRequestServerVenue(int venueId)
{
    settings->setValue("requestServerVenue", venueId);
    emit requestServerVenueChanged(venueId);
}

QString Settings::requestServerApiKey()
{
    return settings->value("requestServerApiKey","").toString();
}

void Settings::setRequestServerApiKey(QString apiKey)
{
    settings->setValue("requestServerApiKey", apiKey);
}

bool Settings::requestServerIgnoreCertErrors()
{
    return settings->value("requestServerIgnoreCertErrors", false).toBool();
}

void Settings::setRequestServerIgnoreCertErrors(bool ignore)
{
    settings->setValue("requestServerIgnoreCertErrors", ignore);
}

bool Settings::audioUseFader()
{
    return settings->value("audioUseFader", true).toBool();
}

bool Settings::audioUseFaderBm()
{
    return settings->value("audioUseFaderBm", true).toBool();
}

void Settings::setAudioUseFader(bool fader)
{
    settings->setValue("audioUseFader", fader);
}

void Settings::setAudioUseFaderBm(bool fader)
{
    settings->setValue("audioUseFaderBm", fader);
}

int Settings::audioVolume()
{
    return settings->value("audioVolume", 50).toInt();
}

void Settings::setAudioVolume(int volume)
{
    settings->setValue("audioVolume", volume);
}

QString Settings::cdgDisplayBackgroundImage()
{
    return settings->value("cdgDisplayBackgroundImage", QString()).toString();
}

void Settings::setCdgDisplayBackgroundImage(QString imageFile)
{
    if (imageFile == "")
        settings->remove("cdgDisplayBackgroundImage");
    else
        settings->setValue("cdgDisplayBackgroundImage", imageFile);
    emit cdgBgImageChanged();
}

Settings::BgMode Settings::bgMode()
{
    return (Settings::BgMode)settings->value("bgMode", 0).toInt();
}

void Settings::setBgMode(Settings::BgMode mode)
{
    settings->setValue("bgMode", mode);
    emit bgModeChanged(mode);
}

QString Settings::bgSlideShowDir()
{
    return settings->value("bgSlideShowDir", QString()).toString();
}

void Settings::setBgSlideShowDir(QString dir)
{
    settings->setValue("bgSlideShowDir", dir);
    emit bgSlideShowDirChanged(dir);
}

bool Settings::audioDownmix()
{
    return settings->value("audioDownmix", false).toBool();
}

void Settings::setAudioDownmix(bool downmix)
{
    settings->setValue("audioDownmix", downmix);
}

bool Settings::audioDownmixBm()
{
    return settings->value("audioDownmixBm", false).toBool();
}

void Settings::setAudioDownmixBm(bool downmix)
{
    settings->setValue("audioDownmixBm", downmix);
}

bool Settings::audioDetectSilence()
{
    return settings->value("audioDetectSilence", false).toBool();
}

bool Settings::audioDetectSilenceBm()
{
    return settings->value("audioDetectSilenceBm", false).toBool();
}

void Settings::setAudioDetectSilence(bool enabled)
{
    settings->setValue("audioDetectSilence", enabled);
}

void Settings::setAudioDetectSilenceBm(bool enabled)
{
    settings->setValue("audioDetectSilenceBm", enabled);
}

QString Settings::audioOutputDevice()
{
    return settings->value("audioOutputDevice", 0).toString();
}

QString Settings::audioOutputDeviceBm()
{
    return settings->value("audioOutputDeviceBm", 0).toString();
}

void Settings::setAudioOutputDevice(QString device)
{
    settings->setValue("audioOutputDevice", device);
}

void Settings::setAudioOutputDeviceBm(QString device)
{
    settings->setValue("audioOutputDeviceBm", device);
}

int Settings::audioBackend()
{
    return settings->value("audioBackend", 0).toInt();
}

void Settings::setAudioBackend(int index)
{
    settings->setValue("audioBackend", index);
    emit audioBackendChanged(index);
}

QString Settings::recordingContainer()
{
    return settings->value("recordingContainer", "ogg").toString();
}

void Settings::setRecordingContainer(QString container)
{
    settings->setValue("recordingContainer", container);
    emit recordingSetupChanged();
}

QString Settings::recordingCodec()
{
    return settings->value("recordingCodec", "undefined").toString();
}

void Settings::setRecordingCodec(QString codec)
{
    settings->setValue("recordingCodec", codec);
    emit recordingSetupChanged();
}

QString Settings::recordingInput()
{
    return settings->value("recordingInput", "undefined").toString();
}

void Settings::setRecordingInput(QString input)
{
    settings->setValue("recordingInput", input);
    emit recordingSetupChanged();
}

QString Settings::recordingOutputDir()
{
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    return settings->value("recordingOutputDir", defaultPath).toString();
}

void Settings::setRecordingOutputDir(QString path)
{
    settings->setValue("recordingOutputDir", path);
    emit recordingSetupChanged();
}

bool Settings::recordingEnabled()
{
    return settings->value("recordingEnabled", false).toBool();
}

void Settings::setRecordingEnabled(bool enabled)
{
    settings->setValue("recordingEnabled", enabled);
    emit recordingSetupChanged();
}

QString Settings::recordingRawExtension()
{
    return settings->value("recordingRawExtension", QString()).toString();
}

void Settings::setRecordingRawExtension(QString extension)
{
    settings->setValue("recordingRawExtension", extension);
}

int Settings::cdgVOffset()
{
    return 0;
    return settings->value("cdgVOffset", 0).toInt();
}

void Settings::setCdgVOffset(int pixels)
{
    settings->setValue("cdgVOffset", pixels);
    emit cdgVOffsetChanged(pixels);
}

int Settings::cdgHOffset()
{
    return 0;
    return settings->value("cdgHOffset", 0).toInt();
}

void Settings::setCdgHOffset(int pixels)
{
    settings->setValue("cdgHOffset", pixels);
    emit cdgHOffsetChanged(pixels);
}

int Settings::cdgVSizeAdjustment()
{
    return 0;
    return settings->value("cdgVSizeAdjustment", 0).toInt();
}

void Settings::setCdgVSizeAdjustment(int pixels)
{
    settings->setValue("cdgVSizeAdjustment", pixels);
    emit cdgVSizeAdjustmentChanged(pixels);
}

int Settings::cdgHSizeAdjustment()
{
    return 0;
    return settings->value("cdgHSizeAdjustment", 0).toInt();
}

void Settings::setCdgHSizeAdjustment(int pixels)
{
    settings->setValue("cdgHSizeAdjustment", pixels);
    emit cdgHSizeAdjustmentChanged(pixels);
}

bool Settings::bmShowFilenames()
{
    return settings->value("showFilenames", false).toBool();
}

void Settings::bmSetShowFilenames(bool show)
{
    settings->setValue("showFilenames", show);
}

bool Settings::bmShowMetadata()
{
    return settings->value("showMetadata", true).toBool();
}

void Settings::bmSetShowMetadata(bool show)
{
    settings->setValue("showMetadata", show);
}

int Settings::bmVolume()
{
    return settings->value("volume", 50).toInt();
}

void Settings::bmSetVolume(int volume)
{
    settings->setValue("volume", volume);
}

int Settings::bmPlaylistIndex()
{
    return settings->value("playlistIndex",0).toInt();
}

void Settings::bmSetPlaylistIndex(int index)
{
    settings->setValue("playlistIndex", index);
}

int Settings::mplxMode()
{
    return settings->value("mplxMode", 0).toInt();
}

void Settings::setMplxMode(int mode)
{
    settings->setValue("mplxMode", mode);
    emit mplxModeChanged(mode);
}
