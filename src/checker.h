#ifndef CHECKER_H
#define CHECKER_H

#include "settings.h"
#include <QString>
#include <QNetworkReply>
#include <QMap>
#include <QFile>
#include <QFileInfo>

class checker : public QObject
{
    Q_OBJECT
public:
    explicit checker(QString sc, QString &startL, QObject *parent = 0);
    qint64 dir_size(const QString &str);
signals:
    void finished(QString);
private slots:
    void comparePaths(QNetworkReply *rep);
    void compareFiles(QNetworkReply *rep);
    void finalCompare();
private:
    QString serverChoosed;
    QString startList;

    QStringList toCheckPath;
    QMap<int, qint64> p_serverSizes;
    QMap<int, qint64> p_clientSizes;

    QStringList toCheckFile;
    QMap<int, qint64> f_serverSizes;
    QMap<int, qint64> f_clientSizes;

    QStringList toDownload;
    settings sett;
};

#endif // CHECKER_H
