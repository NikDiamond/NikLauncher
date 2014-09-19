#include "mainclass.h"
#include "connecter.h"
#include "downloader.h"
#include "checker.h"
#include "mdparser.h"
#include "ui_mainclass.h"
#include "minecraft.h"
#include "reg.h"

MainClass::MainClass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainClass)
{
    ui->setupUi(this);
    setUi();

    updateServers();
    getServers();
    checkPath();
}

MainClass::~MainClass()
{delete ui;}

void MainClass::getServers(){
    connecter *getServersConnect = new connecter("servers.php");
    getServersConnect->doConnect();
    connect(getServersConnect, SIGNAL(ready(QNetworkReply*)), this, SLOT(serversWrite(QNetworkReply*)));
}

void MainClass::extract(QString p, QString outP)
{
    QZipReader *reader = new QZipReader(p);
    reader->extractAll(outP);
}

void MainClass::startGame()
{
    //Запуск игры
    status("Запуск игры...");
    minecraft *game = new minecraft(serverChoosed, login, pass, access, serverIp, serverPort);
    connect(game, SIGNAL(started()), this, SLOT(close()));
    game->launch();
}

void MainClass::uiEnabled(bool stat)
{
    qDebug() << "Ui switched to " << stat;
    ui->toGame->setEnabled(stat);
    ui->toReg->setEnabled(stat);
    ui->comboBox->setEnabled(stat);
}
void MainClass::updateServers(){
    ui->comboBox->clear();
    for(int i=0;i<sett.serversList.count();i++){
        ui->comboBox->addItem(sett.serversList[i][0]);
    }
}

void MainClass::checkPath(QString path)
{
    QDir mainDir(sett.globalPath+path);
    if(!mainDir.exists()){
        QDir().mkdir(sett.globalPath+path);
        qDebug() << "Path " + sett.globalPath+path + " was created!";
    }
}

void MainClass::setUi(){
    setLayout(ui->mainLayout);
    setMaximumSize(width(),height());
    setMinimumSize(width(),height());

    QPalette pal = palette();
    pal.setBrush(QPalette::Window, QBrush(QPixmap( ":/img/images/mainBack.png")));
    setPalette(pal);
    setWindowFlags(Qt::FramelessWindowHint);


    connect(ui->loginField, SIGNAL(returnPressed()), ui->toGame, SLOT(click()));
    connect(ui->passField, SIGNAL(returnPressed()), ui->toGame, SLOT(click()));
}

void MainClass::mousePressEvent(QMouseEvent *event)
{
    // Запоминаем позицию при нажатии кнопки мыши
    mpos = event->pos();
}

void MainClass::mouseMoveEvent(QMouseEvent *event)
{
    if (mpos.x() >= 0 && mpos.y() <= 50 && event->buttons() && Qt::LeftButton)
    {
        QPoint diff = event->pos() - mpos;
        QPoint newpos = this->pos() + diff;
        this->move(newpos);
    }
}

void MainClass::mouseReleaseEvent(QMouseEvent *)
{
    // Очищаем старое значение позиции нажатия мыши
    mpos = QPoint(-1, -1);
}

void MainClass::status(QString err)
{
    ui->errLabel->setText(err);
}

void MainClass::serversWrite(QNetworkReply *rep){
    QString rp = rep->readAll();
    QStringList rpList = rp.simplified().split("<>");
    if(rpList[0] != ""){
        qDebug() << "writing to servers list...";

        //Записываем список серверов в QMap
        for(int i=0;i<rpList.count();i++){
            sett.serversList[i] << rpList[i].simplified().split(",");
        }
        //Избавляемся от пробелов
        for(int i=0;i<sett.serversList.count();i++){
            for(int j=0;j<sett.serversList[i].count();j++){
                sett.serversList[i][j] = sett.serversList[i][j].simplified();
            }
        }
        //Обновляем список
        updateServers();
    }else{
        qDebug() << "Server is unavailable.";
        status("Сервер недоступен!");
    }
}

void MainClass::checkFiles(QNetworkReply *rep)
{
    filesList = rep->readAll();
    //check if download needable
    checker *chckFiles = new checker(serverChoosed, filesList);
    connect(chckFiles, SIGNAL(finished(QString)), this, SLOT(downloadClient(QString)));
    connect(chckFiles, SIGNAL(clientNotFound()), this, SLOT(clientNotFound()));
}

void MainClass::clientNotFound(){
    status("Клиент не найден! (Ошибка сервера)");
    uiEnabled(true);
}

void MainClass::downloadClient(QString newList)
{
    filesList = newList;
    totalDownSize = 0;

    connecter *connT = new connecter("launcher.php");
    connT->doConnect("act=filesSize&server=" + serverChoosed + "&files=" + filesList);
    connect(connT, SIGNAL(ready(QNetworkReply*)), this, SLOT(downloadStart(QNetworkReply*)));
}

void MainClass::downloadStart(QNetworkReply *repl)
{
    status("Скачивание клиента...");
    QString rp = repl->readAll();

    totalDownSize = rp.toLongLong();
    qDebug() << "Total size: " << totalDownSize;
    if(totalDownSize == 0)
        qDebug() << rp.toInt() << " or " << rp;

    QStringList list = filesList.simplified().split(",");
    qDebug() << list;

    downClient = new downloader(serverChoosed);
    downClient->append(list);

    prevR = 0;
    connect(downClient, SIGNAL(finished()), this, SLOT(clientDownloaded()));
    connect(downClient, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64)));
}

void MainClass::showUi()
{
    uiEnabled(true);
}

void MainClass::downloadError()
{
    status("Ошибка загрузки!");
}

void MainClass::downloadProgress(qint64 r)
{
    qint64 plus = r-prevR;
    if(r < prevR)
        plus = r;
    qint64 newVal = plus + ui->downProgress->value();

    QString received = QString::number(newVal/1024);
    QString total = QString::number(totalDownSize/1024);

    ui->downProgress->setMaximum(totalDownSize);
    ui->downProgress->setValue(newVal);

    status(received.left(received.indexOf(received.right(3))) + " " + received.right(3) +" / "+ total.left(total.indexOf(total.right(3))) + " " + total.right(3) +" KB");

    prevR = r;
    ui->downProgress->repaint();
}

void MainClass::clientDownloaded()
{
    prevR = 0;
    downClient->deleteLater();
    status("Клиент скачан!");
    startGame();
}

void MainClass::on_toGame_clicked()
{
    ui->downProgress->setValue(0);
    QString login = ui->loginField->text().simplified();
    QString pass = ui->passField->text().simplified();

    if(login.length()<6 || login.length()>15){status("Неверный логин."); return;}
    if(pass.length()<6){status("Неверный пароль."); return;}

    uiEnabled(false);
    status("Загрузка...");
    connecter *authConn = new connecter("auth.php");
    authConn->doConnect("act=auth&login="+login+"&password="+mdparser::parse(pass));
    connect(authConn, SIGNAL(ready(QNetworkReply*)), this, SLOT(startUserParsing(QNetworkReply*)));
}

void MainClass::startUserParsing(QNetworkReply *rep)
{
    QString rp = rep->readAll();
    qDebug() << rp;

    if(rp.contains("<>")){
        QStringList loginData = rp.split("<>");
        login = loginData[1];
        access = loginData[2];
        pass = loginData[0];
        serverChoosed = ui->comboBox->currentText();

        QStringList serverNow;
        for(int i=0;i<sett.serversList.count();i++){
            if(sett.serversList[i][0] == serverChoosed){
                serverNow = sett.serversList[i];
            }
        }
        if(serverNow.count() == 0){
            status("Ошибка выбора сервера");
            return;
        }
        serverIp = serverNow[1];
        serverPort = serverNow[2];

        status("Проверка файлов...");
        checkPath();
        checkPath("/" + serverChoosed);;

        connecter *getFilesList = new connecter("launcher.php");
        getFilesList->doConnect("act=filesList&server="+serverChoosed);
        connect(getFilesList, SIGNAL(ready(QNetworkReply*)), this, SLOT(checkFiles(QNetworkReply*)));
        //go to files checking and download
    }else{
        uiEnabled(true);
        status(rp);
        return;
    }
}

void MainClass::on_toReg_clicked()
{
    reg *regForm = new reg(this);
    connect(regForm, SIGNAL(closed()), this, SLOT(showUi()));
    regForm->setGeometry(20,80,0,0);
    regForm->setMaximumSize(460,360);
    regForm->setMinimumSize(460,360);
    uiEnabled(false);
    regForm->show();
    regForm->setUi();
}

void MainClass::on_toClose_clicked()
{
    close();
}
