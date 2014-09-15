#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QStringList>
#include <QStandardPaths>
#include <QMap>
#include <QDebug>

class settings
{
public:
    settings();
    QString clientVer;
    QString domain;
    QString launcherName;
    QString globalPath;
    QString pathOnSite;
    QMap<int, QStringList> serversList;
};

#endif // SETTINGS_H
