#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QStringList>
#include <QStandardPaths>

class settings
{
public:
    settings();
    QString clientVer;
    QString domain;
    QString launcherName;
    QString globalPath;
    QStringList serversList;
};

#endif // SETTINGS_H
