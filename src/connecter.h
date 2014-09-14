#ifndef CONNECTER_H
#define CONNECTER_H

#include <QUrl>
#include <QString>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "settings.h"

class connecter : public QObject
{
    Q_OBJECT

public:
    explicit connecter(QString u, QObject *parent = 0);
    ~connecter();

    void doConnect(QString params="");
private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QString url;
    settings sett;
private slots:
    void gotRequest(QNetworkReply *rep);
signals:
    void ready(QNetworkReply *rep);
};

#endif // CONNECTER_H
