#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
    log("Launcher started.");
    //LAUNCHER SETTINGS//

    isDebug = true;                                 //Выводить отладочные сообщения [true/false]
    autoJoin = false;                               //Автозаход на сервер
    domain = "nikdiamond.hol.es";                   //Адрес папки с файлами лаунчера на сайте
    clientVersion = "0.1";

    //LAUNCHER SETTINGS//

    //UI SET
    ui->setupUi(this);

    ui->password->setEchoMode(QLineEdit::Password);

    this->setMinimumSize(this->width(),this->height());
    this->setMaximumSize(this->width(), this->height());
    //UI SET
    getServers();
    connect(ui->userName, SIGNAL(returnPressed()), ui->startGame, SLOT(click()));
    connect(ui->password, SIGNAL(returnPressed()), ui->startGame, SLOT(click()));
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::log(QString msg){
    if(isDebug)
        qDebug() << msg;
}

void MainWindow::log(QStringList msg){
    if(isDebug)
        qDebug() << msg;
}

void MainWindow::writeOut(){//Получаем отладочные данные из Java
    QString error = proc->readAllStandardError();
    QString output = proc->readAllStandardOutput();
    QString msg;

    if(error != "")  msg = error;
    if(output != "") msg = output;
    log("[GAME]"+msg.simplified());
}
void MainWindow::validatePath(){
    log("Validating game files...");
    QString gamePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).at(0) + "\\NikLauncher";
    QDir path(gamePath);

    if(!path.exists()){//Если нет папки лаунчера, создаём
        path.mkdir(gamePath);
        log("Game path doesn't exists. Creating...");
    }
    log("Game path = "+gamePath);
}

void MainWindow::getServers(){
    //создаём коннект-мэнеджер
    manager = new QNetworkAccessManager(this);
    QUrl url = "http://"+domain+"/servers.php";
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/html; charset=utf-8");
    request.setHeader(QNetworkRequest::UserAgentHeader, clientVersion);

    //посылаем запрос
    log("Updating servers list...");
    serversGotReply = manager->get(request);
    connect(serversGotReply, SIGNAL(readyRead()), this, SLOT(serversGot()));
}

void MainWindow::serversGot(){
    QString servers = serversGotReply->readAll();
    QStringList serversList = servers.simplified().split("<>");
    QComboBox *serversBox = ui->serversList;
    serversBox->clear();

    for(int i=0; i<serversList.count()-1;i++){
        serversList[i] = serversList[i].simplified();
        QStringList serverData = serversList[i].split(",");
        serversBox->addItem(serverData[0]);
        for(int j=0;j<serverData.count();j++){
            serverData[j] = serverData[j].simplified();
            serversMap[i] << serverData[j];//Заносим данные сервера в массив серверов
        }
    }
    ui->startGame->setEnabled(true);
}

void MainWindow::startGame(){
    log("Starting Java Virtual Mashine...");
    //Зоздаём процесс
    proc = new QProcess(this);
    QStringList args;

    QStringList serverData;
    QString text = ui->serversList->currentText();
    for(int i=0; i<serversMap.count();i++){
        if(serversMap[i][0] == text){
            serverData = serversMap[i];
        }
    }
    log("Server choosed: " + serverData[0]);

    //Java аргументы
    args << "-XX:HeapDumpPath=MojangTricksIntelDriversForPerformance_javaw.exe_minecraft.exe.heapdump";
    args << "-Xmx1G";
    args << "-XX:+UseConcMarkSweepGC";
    args << "-XX:+CMSIncrementalMode";
    args << "-XX:-UseAdaptiveSizePolicy";
    args << "-Xmn128M";

    //Указываем пути библиотек
    args << "-Djava.library.path=C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\versions\\1.8\\natives";
    args << "-cp" << "C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\java3d\\vecmath\\1.5.2\\vecmath-1.5.2.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\net\\sf\\trove4j\\trove4j\\3.0.3\\trove4j-3.0.3.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\com\\ibm\\icu\\icu4j-core-mojang\\51.2\\icu4j-core-mojang-51.2.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\net\\sf\\jopt-simple\\jopt-simple\\4.6\\jopt-simple-4.6.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\com\\paulscode\\codecjorbis\\20101023\\codecjorbis-20101023.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\com\\paulscode\\codecwav\\20101023\\codecwav-20101023.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\com\\paulscode\\libraryjavasound\\20101123\\libraryjavasound-20101123.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\com\\paulscode\\librarylwjglopenal\\20100824\\librarylwjglopenal-20100824.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\com\\paulscode\\soundsystem\\20120107\\soundsystem-20120107.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\io\\netty\\netty-all\\4.0.15.Final\\netty-all-4.0.15.Final.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\com\\google\\guava\\guava\\17.0\\guava-17.0.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\org\\apache\\commons\\commons-lang3\\3.3.2\\commons-lang3-3.3.2.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\commons-io\\commons-io\\2.4\\commons-io-2.4.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\commons-codec\\commons-codec\\1.9\\commons-codec-1.9.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\net\\java\\jinput\\jinput\\2.0.5\\jinput-2.0.5.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\net\\java\\jutils\\jutils\\1.0.0\\jutils-1.0.0.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\com\\google\\code\\gson\\gson\\2.2.4\\gson-2.2.4.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\com\\mojang\\authlib\\1.5.17\\authlib-1.5.17.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\com\\mojang\\realms\\1.5.4\\realms-1.5.4.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\org\\apache\\commons\\commons-compress\\1.8.1\\commons-compress-1.8.1.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\org\\apache\\httpcomponents\\httpclient\\4.3.3\\httpclient-4.3.3.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\commons-logging\\commons-logging\\1.1.3\\commons-logging-1.1.3.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\org\\apache\\httpcomponents\\httpcore\\4.3.2\\httpcore-4.3.2.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\org\\apache\\logging\\log4j\\log4j-api\\2.0-beta9\\log4j-api-2.0-beta9.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\org\\apache\\logging\\log4j\\log4j-core\\2.0-beta9\\log4j-core-2.0-beta9.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\org\\lwjgl\\lwjgl\\lwjgl\\2.9.1\\lwjgl-2.9.1.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\org\\lwjgl\\lwjgl\\lwjgl_util\\2.9.1\\lwjgl_util-2.9.1.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\libraries\\tv\\twitch\\twitch\\6.5\\twitch-6.5.jar;C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\versions\\1.8\\1.8.jar";

    //Вызываем главный класс
    args << "net.minecraft.client.main.Main";

    //Передаём аргументы в Minecraft
    args << "--username" << authData[1];
    args << "--version" << "1.8";
    args << "--gameDir" << "C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\";
    args << "--assetsDir" << "C:\\Users\\NikDiamond\\AppData\\Roaming\\.minecraft\\assets\\";
    args << "--assetIndex" << "1.8";
    args << "--uuid" << "";
    args << "--accessToken" << authData[2];
    args << "--userProperties" << "{}";
    args << "--userType" << "legacy";
    if(autoJoin){
        args << "--server" << serverData[1];
        args << "--port" << serverData[2];
    }

    proc->execute("C:\\Program Files\\Java\\jre7\\bin\\javaw.exe",  args);
    close();
}

void MainWindow::auth(){
    //создаём коннект-мэнеджер
    manager = new QNetworkAccessManager(this);

    //записываем нужные параметры в строку
    QCryptographicHash md5Generator(QCryptographicHash::Md5);
    md5Generator.addData(password.toUtf8());
    QString passMd5 = md5Generator.result().toHex();

    QByteArray params = QString("act=auth&login="+userName+"&password="+passMd5).toUtf8();
    QUrl url = "http://"+domain+"/auth.php";
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::UserAgentHeader, clientVersion);

    //посылаем запрос
    log("Sending POST query to "+url.toString()+" "+params+" ...");
    authReply = manager->post(request, params);
    connect(authReply, SIGNAL(readyRead()), this, SLOT(authGotReply()));
}
void MainWindow::registration(){
    if(userName == "" || password == ""){
        QMessageBox msgBox;
        msgBox.setInformativeText("Вы не ввели имя пользователя или пароль. Пожалуйста, введите данные и попробуйте ещё раз.");
        msgBox.exec();
        return;
    }
    if(userName.length() < 5 || userName.length() > 15){
        QMessageBox msgBox;
        msgBox.setInformativeText("Логин должен состоять из 5-15 символов.");
        msgBox.exec();
        return;
    }
    if(password.length() < 6){
        QMessageBox msgBox;
        msgBox.setInformativeText("Слишком короткий пароль. (минимум 6 символов)");
        msgBox.exec();
        return;
    }

    //создаём коннект-мэнеджер
    manager = new QNetworkAccessManager(this);

    //записываем нужные параметры в строку
    QCryptographicHash md5Generator(QCryptographicHash::Md5);
    md5Generator.addData(password.toUtf8());
    QString passMd5 = md5Generator.result().toHex();

    QByteArray params = QString("act=reg&login="+userName+"&password="+passMd5).toUtf8();
    QUrl url = "http://"+domain+"/auth.php";
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::UserAgentHeader, clientVersion);

    //посылаем запрос
    log("Sending POST query to "+url.toString()+" "+params+" ...");
    regReply = manager->post(request, params);
    connect(regReply, SIGNAL(readyRead()), this, SLOT(regGotReply()));
}

void MainWindow::authGotReply(){//когда получен ответ от скрипта авторизации
    QString result = authReply->readAll().simplified();

    log("Server reply: " + result);
    if(result == "queryError" || result == "sqlError" || result == "Bad Login" || result == "LoginPassError"){
        QMessageBox msgBox;
        msgBox.setInformativeText(result);
        msgBox.exec();
        return;
    }else{//всё нормально
        authData = result.split("<>");
        log(authData);
        startGame();
    }
}

void MainWindow::regGotReply(){//когда получен ответ от скрипта авторизации
    QString result = regReply->readAll().simplified();

    log("Server reply: " + result);
    if(result == "userExists"){//Если пользователь уже существует
        QMessageBox msgBox;
        msgBox.setInformativeText("Пользователь с таким логином уже существует!");
        msgBox.exec();
        return;
    }

    userName = "";
    password = "";
    ui->userName->clear();
    ui->password->clear();
}

void MainWindow::on_startGame_clicked()
{
    log("Checking data before start game");

    validatePath();//проверяем файлы
    userName = ui->userName->text();
    password = ui->password->text();

    if(userName == "" || password == ""){//Если ник не введён
        QMessageBox msgBox;
        msgBox.setInformativeText("Вы не ввели имя пользователя или пароль. Пожалуйста, введите данные и попробуйте ещё раз.");
        msgBox.exec();
        return;
    }
    //Авторизируемся
    auth();
}

void MainWindow::on_registration_clicked()
{
    //Регистрация пользователя
    log("Registration.");
    userName = ui->userName->text();
    password = ui->password->text();

    registration();
}
