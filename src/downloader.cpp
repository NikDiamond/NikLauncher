#include "downloader.h"

downloader::downloader(QString sc, QObject *parent) :
    QObject(parent)
{
    downloadedCount = 0;
    totalCount = 0;
    count = 0;
    serverChoosed = sc;
    url = "http://"+sett.domain+"/"+sett.pathOnSite;
    filePath = sett.globalPath;

    file = new QFile(filePath);
    if(!file->open(QIODevice::WriteOnly))
        emit downError();

    manager = new QNetworkAccessManager(this);
    currentDownload = manager->get(QNetworkRequest(url));
}
downloader::~downloader(){
    file->deleteLater();
    manager->deleteLater();
}

void downloader::append(const QUrl &url)
{
    if (downloadQueue.isEmpty())
        QTimer::singleShot(0, this, SLOT(tstartNextDownload()));

    downloadQueue.enqueue(url);
    ++totalCount;
}

void downloader::append(const QStringList &urlList)
{
    foreach (QString url, urlList){
        paths[totalCount] = url;
        url = "http://" + sett.domain + "/" + sett.pathOnSite+ "/clients/" + serverChoosed + "/" + url;
        append(QUrl::fromEncoded(url.toLocal8Bit()));
    }

    if (downloadQueue.isEmpty())
        QTimer::singleShot(0, this, SIGNAL(finished()));
}

void downloader::tstartNextDownload()
{
    if (downloadQueue.isEmpty()) {
        qDebug() << "Files downloaded successfully" << downloadedCount << "/" << totalCount;
        emit finished();
        return;
    }

    QUrl url = downloadQueue.dequeue();
    QString downPath = sett.globalPath + serverChoosed + "/" + paths[count];

    QDir downDir = QFileInfo(downPath).absoluteDir();
    if(!downDir.exists()){//Создаём папку для файла, если её нет
        downDir.mkdir(downDir.absolutePath());
        qDebug() << "Path " << downDir.absolutePath() << " created!";
    }

    file = new QFile(downPath);
    if(!file->open(QIODevice::WriteOnly)){
        qDebug() << "Can't download file " << url << " to " << downPath;
        tstartNextDownload();
        return;
    }

    manager = new QNetworkAccessManager();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, sett.clientVer);
    currentDownload = manager->get(request);

    connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)), SLOT(tdownloadProgress(qint64,qint64)));
    connect(currentDownload, SIGNAL(finished()), SLOT(tdownloadFinished()));
    connect(currentDownload, SIGNAL(readyRead()), SLOT(tdownloadReadyRead()));

    qDebug() << "Downloading " << url.toEncoded().constData() << " ...";
    downloadTime.start();
}

void downloader::tdownloadReadyRead()
{
    if(file)
        file->write(currentDownload->readAll());
}

void downloader::tdownloadProgress(qint64 rec, qint64 tot)
{
    emit downloadProgress(rec, tot);
}

void downloader::tdownloadFinished()
{
    file->close();
    if(QFileInfo(sett.globalPath + serverChoosed + "/" + paths[count]).completeSuffix() == "zip"){
        QZipReader *reader = new QZipReader(sett.globalPath + serverChoosed + "/" + paths[count]);
        reader->extractAll(QFileInfo(sett.globalPath + serverChoosed + "/" + paths[count]).absoluteDir().absolutePath());
        reader->close();
        QFile *remFile = new QFile(sett.globalPath + serverChoosed + "/" + paths[count]);
        if(remFile->open(QIODevice::ReadWrite)){
            remFile->remove();
        }else{
            remFile->close();
        }
    }
    ++count;
    ++downloadedCount;
    tstartNextDownload();
}
