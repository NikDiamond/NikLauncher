#ifndef MINECRAFT_H
#define MINECRAFT_H

#include <QObject>
#include <QDebug>
#include <QProcess>
#include "settings.h"

class minecraft : public QObject
{
    Q_OBJECT
public:
    explicit minecraft(QString sc, QString login, QString pass, QString access, QString server, QString port, QObject *parent = 0);
    void launch();

signals:
    void started();
public slots:
    void startedSlot();
private:
    QProcess *proc;
    QString serverChoosen;
    QString login;
    QString password;
    QString accessToken;
    QString serverIp;
    QString serverPort;

    settings sett;

};

#endif // MINECRAFT_H
