#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QUrl>
#include <QString>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QByteArray>
#include <QFile>
#include "settings.h"

class downloader : public QObject
{
    Q_OBJECT
public:
    explicit downloader(QString u, QString p, QObject *parent = 0);
    ~downloader();
private:
    QUrl url;
    QString filePath;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QFile *file;
    settings sett;
private slots:
    void tReadyRead();
    void tdownloadProgress(qint64 rec, qint64 tot);
    void tFinished();
signals:
    void downError();
    void downloadProgress(qint64, qint64);
    void finished();
};

#endif // DOWNLOADER_H
