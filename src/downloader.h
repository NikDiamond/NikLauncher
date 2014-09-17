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
#include <QFileInfo>
#include <QDir>
#include <QQueue>
#include <QTime>
#include <QTimer>
#include "settings.h"
#include <private/qzipreader_p.h>
#include <private/qzipwriter_p.h>

class downloader : public QObject
{
    Q_OBJECT
public:
    explicit downloader(QString sc, QObject *parent = 0);
    ~downloader();

    void append(const QUrl &url);
    void append(const QStringList &urlList);
    void checkPath(QString path);
private:
    QUrl url;
    QString filePath;
    settings sett;
    QFile *file;
    QString serverChoosed;

    int count;
    QMap<int,QString> paths;

    QNetworkAccessManager *manager;
    QNetworkReply *currentDownload;
    QQueue<QUrl> downloadQueue;
    QTime downloadTime;

    qint64 totalSize;
    int downloadedCount;
    int totalCount;
private slots:
    void tstartNextDownload();
    void tdownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void tdownloadFinished();
    void tdownloadReadyRead();
signals:
    void downError();
    void downloadProgress(qint64, qint64);
    void finished();
};

#endif // DOWNLOADER_H
