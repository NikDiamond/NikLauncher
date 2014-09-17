#include "checker.h"
#include "connecter.h"
#include "downloader.h"

checker::checker(QString sc, QString &startL, QObject *parent) :
    QObject(parent),serverChoosed(sc),startList(startL)
{
    toCheckPath << "bin/libraries";
    toCheckPath << "bin/natives";

    toCheckFile << "bin/minecraft.jar";

    connecter *getPathSizes = new connecter("launcher.php");
    getPathSizes->doConnect("act=pathSizes&server="+serverChoosed+"&path="+QString(toCheckPath.join("-")));
    connect(getPathSizes, SIGNAL(ready(QNetworkReply*)), this, SLOT(comparePaths(QNetworkReply*)));
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

void checker::comparePaths(QNetworkReply *rep)
{
    //server-side
    QString rp = rep->readAll();
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

    connecter *getFileSizes = new connecter("launcher.php");
    getFileSizes->doConnect("act=fileSizes&server="+serverChoosed+"&file="+QString(toCheckFile.join("-")));
    connect(getFileSizes, SIGNAL(ready(QNetworkReply*)), this, SLOT(compareFiles(QNetworkReply*)));
}

void checker::compareFiles(QNetworkReply *rep)
{
    //server-side
    QString rp = rep->readAll();

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
            qDebug() << f_serverSizes[i] << " ../.. " << f_clientSizes[i];
        }
    }
    toDownload << "config.zip";
    finalCompare();
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
