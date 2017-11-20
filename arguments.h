#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <QString>

class Arguments
{
public:
    Arguments();
    int monitor;
    QString cdg_file;
    QString mp3_file;
    QString getMp3();
    QString getCDG();
    int getMonitor();
};

#endif // ARGUMENTS_H
