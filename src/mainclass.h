#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QLayout>
#include <QLineEdit>
#include <QNetworkReply>
#include <QMouseEvent>
#include <QStandardPaths>
#include <QMap>
#include <QDir>
#include <QByteArray>
#include "settings.h"

namespace Ui {
class MainClass;
}

class MainClass : public QWidget
{
    Q_OBJECT

public:
    explicit MainClass(QWidget *parent = 0);
    void setUi();
    void status(QString err);
    ~MainClass();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void updateServers();
    void checkPath(QString path = "");
    void getServers();
    void extract(QString p, QString outP);
    void startGame();
    void uiEnabled(bool stat);
private slots:
    void on_toGame_clicked();
    void startUserParsing(QNetworkReply *rep);

    void on_toReg_clicked();

    void on_toClose_clicked();

    void serversWrite(QNetworkReply *rep);

    void checkFiles(QNetworkReply *rep);

    void downloadClient(QString newList);

    void downloadError();

    void downloadProgress(qint64 r);

    void clientDownloaded();

    void downloadStart(QNetworkReply *repl);

private:
    Ui::MainClass *ui;
    QPoint mpos;
    settings sett;

    QString serverChoosed;
    QString clientFilePath;
    qint64 totalDownSize;
    QString filesList;
    qint64 prevR;

    QString login;
    QString pass;
    QString access;
    QString serverIp;
    QString serverPort;
};

#endif // MAINCLASS_H
