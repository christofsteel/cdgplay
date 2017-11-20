#include "arguments.h"
#include <QApplication>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDesktopWidget>

Arguments::Arguments()
{
    this->monitor = -1;
    this->cdg_file = "";
}

int Arguments::getMonitor() {
    if(this->monitor == -1) {
        return QApplication::desktop()->screenCount() - 1;
    } else {
        return this->monitor;
    }
}

QString Arguments::getCDG() {
    return this->cdg_file;
}

QString Arguments::getMp3() {
    int lastPoint = this->cdg_file.lastIndexOf(".");
    return this->cdg_file.left(lastPoint) + ".mp3";
}
