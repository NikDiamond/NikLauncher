#include "downloader.h"

downloader::downloader(QString u, QString p, QObject *parent) :
    QObject(parent)
{
    url = "http://"+sett.domain+"/"+sett.pathOnSite+u;
    filePath = sett.globalPath+p;

    manager = new QNetworkAccessManager(this);
    reply = manager->get(QNetworkRequest(url));

    qDebug() << url;

    file = new QFile(filePath);
    if(!file->open(QIODevice::WriteOnly))
        emit downError();
    qDebug() << "Start downloading...";
    connect(reply, SIGNAL(readyRead()), this, SLOT(tReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(tdownloadProgress(qint64,qint64)));
    connect(reply, SIGNAL(finished()), this, SLOT(tFinished()));
}
downloader::~downloader(){
    file->deleteLater();
    manager->deleteLater();
}

void downloader::tReadyRead()
{
    if(file)
        file->write(reply->readAll());
}

void downloader::tdownloadProgress(qint64 rec, qint64 tot)
{
    emit downloadProgress(rec, tot);
}

void downloader::tFinished()
{
    file->close();
    emit finished();
    this->deleteLater();
}
