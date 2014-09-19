#include "checker.h"
#include "connecter.h"
#include "downloader.h"

void checker::AddToHash(const QFileInfo& fileInf, QCryptographicHash& cryptHash)
{
    QDir directory(fileInf.absoluteFilePath());
    directory.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    QFileInfoList fileInfoList = directory.entryInfoList();

    foreach(QFileInfo info, fileInfoList)
    {
        if(info.isDir())
        {
            // recurse through all directories
            AddToHash(info, cryptHash);
            continue;
        }

        // add all file contents to the hash
        if(info.isFile())
        {
            QFile file(info.absoluteFilePath());
            if(!file.open(QIODevice::ReadOnly))
            {
                // failed to open file, so skip
                continue;
            }
            cryptHash.addData(&file);
            file.close();
        }
    }
}

qint64 checker:: dir_size(const QString & str)
{
    quint64 sizex = 0;
    QFileInfo str_info(str);
    if (str_info.isDir())
    {
        QDir dir(str);
        QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs |  QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        for (int i = 0; i < list.size(); ++i)
        {
            QFileInfo fileInfo = list.at(i);
            if(fileInfo.isDir())
            {
                    sizex += dir_size(fileInfo.absoluteFilePath());
            }
            else
                sizex += fileInfo.size();
        }
    }
    return sizex;
}

checker::checker(QString sc, QString &startL, QObject *parent) :
    QObject(parent),serverChoosed(sc),startList(startL)
{

    toCheckFile << "bin/minecraft.jar";

    toCheckArchive << "bin/natives.zip";
    toCheckArchive << "bin/assets.zip";
    toCheckArchive << "bin/libraries.zip";

    connecter *chkClientOnServer = new connecter("launcher.php");//проверяем наличие клиента на сервере
    chkClientOnServer->doConnect("act=checkClient&server="+serverChoosed);
    connect(chkClientOnServer, SIGNAL(ready(QNetworkReply*)), this, SLOT(checkClient(QNetworkReply*)));
}

void checker::checkClient(QNetworkReply *rep)
{
    QString rp = rep->readAll();
    if(rp == "error"){
        emit clientNotFound();//Клиент не найден
        return;
    }
    connecter *getPathSizes = new connecter("launcher.php");

    QString send = "";
    if(toCheckPath.count() != 0){
        send = QString(toCheckPath.join("-"));
    }
    getPathSizes->doConnect("act=pathSizes&server="+serverChoosed+"&path="+send);
    connect(getPathSizes, SIGNAL(ready(QNetworkReply*)), this, SLOT(comparePaths(QNetworkReply*)));
}

void checker::comparePaths(QNetworkReply *rep)
{
    //server-side
    QString rp = rep->readAll();
    if(rp != "error"){
        if(rp == ""){
            qDebug() << "wrong php-script";
        }
        qDebug() << "Comparing paths";

        QStringList sizes = rp.simplified().split(",");
        for(int i=0;i<sizes.count()-1;i++){
            QStringList oneSize = sizes[i].simplified().split("-");
            p_serverSizes[i] = oneSize[1].toLongLong();
        }

        //client-side
        for(int i=0;i<toCheckPath.count();i++){
            p_clientSizes[i] = dir_size(sett.globalPath+serverChoosed+"/"+toCheckPath[i]);
        }

        //compairing
        for(int i=0;i<sizes.count()-1;i++){
            if(p_serverSizes[i] != p_clientSizes[i]){
                toDownload << toCheckPath[i];
            }
        }
    }
    connecter *getFileSizes = new connecter("launcher.php");
    QString send = "";
    if(toCheckFile.count() != 0){
        send = QString(toCheckFile.join("-"));
    }
    getFileSizes->doConnect("act=fileSizes&server="+serverChoosed+"&file="+send);
    connect(getFileSizes, SIGNAL(ready(QNetworkReply*)), this, SLOT(compareFiles(QNetworkReply*)));
}

void checker::compareFiles(QNetworkReply *rep)
{
    //server-side
    QString rp = rep->readAll();
    if(rp != "error"){
        if(rp == ""){
            qDebug() << "wrong php-script";
        }
        qDebug() << "Comparing files";

        QStringList sizes = rp.simplified().split(",");
        for(int i=0;i<sizes.count()-1;i++){
            QStringList oneSize = sizes[i].simplified().split("-");
            f_serverSizes[i] = oneSize[1].toLongLong();
        }

        //client-side
        for(int i=0;i<toCheckFile.count();i++){
            QFile file(sett.globalPath+serverChoosed+"/"+toCheckFile[i]);
            file.open(QIODevice::ReadOnly);
            f_clientSizes[i] = file.size();
        }

        //comparing
        for(int i=0;i<sizes.count()-1;i++){
            if(f_serverSizes[i] != f_clientSizes[i]){
                toDownload << toCheckFile[i];
            }
        }
        toDownload << "config.zip";
    }
    connecter *getFileSizes = new connecter("launcher.php");
    QString send = "";
    if(toCheckArchive.count() != 0){
        send = QString(toCheckArchive.join(","));
    }
    getFileSizes->doConnect("act=origZipsSize&server="+serverChoosed+"&files="+send);
    connect(getFileSizes, SIGNAL(ready(QNetworkReply*)), this, SLOT(compareArchive(QNetworkReply*)));
}

void checker::compareArchive(QNetworkReply *rep)
{
    QString rp = rep->readAll();
    if(rp != "error"){
        if(rp == ""){
            qDebug() << "wrong php-script";
        }
        qDebug() << "Comparing archives";

        QStringList sizes = rp.simplified().split(",");
        for(int i=0;i<sizes.count()-1;i++){
            QStringList size = sizes[i].simplified().split("-");

            QString client_sideName = sett.globalPath + serverChoosed + "/" + QFileInfo(size[0]).path() + "/" + QFileInfo(size[0]).baseName();
            qint64 size_client = dir_size(client_sideName);

            if(size[1].toInt() != size_client)
                toDownload << size[0];
        }
        emit finalCompare();
    }
}

void checker::finalCompare()
{
    QStringList startFiles = startList.simplified().split(",");
    QStringList newList;

    for(int j=0;j<toDownload.count();j++){
        for(int i=0;i<startFiles.count();i++){
            if(startFiles[i].contains(toDownload[j])){
                newList << startFiles[i];
            }
        }
    }
    startList = newList.join(",");
    emit finished(startList);
}
