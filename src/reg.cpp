#include "reg.h"
#include "connecter.h"
#include "ui_reg.h"
#include "mdparser.h"

reg::reg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::reg)
{
    ui->setupUi(this);
}

reg::~reg()
{
    delete ui;
}

void reg::setUi()
{
    setLayout(ui->regLayout);
    ui->regWrap->setObjectName("regWindow");
    ui->regWrap->setStyleSheet("#regWindow{background-image: url(:/img/images/regBack.png);}");
    ui->regWrap->setMaximumSize(maximumWidth(),maximumHeight());
    ui->regWrap->setMinimumSize(maximumWidth(),maximumHeight());

    connect(ui->loginField, SIGNAL(returnPressed()), ui->doReg, SLOT(click()));
    connect(ui->passField, SIGNAL(returnPressed()), ui->doReg, SLOT(click()));
    connect(ui->passField2, SIGNAL(returnPressed()), ui->doReg, SLOT(click()));
}

void reg::error(QString err)
{
    ui->errLabel->setText(err);
}

void reg::on_toBack_clicked()
{
    this->hide();
    this->deleteLater();
}

void reg::on_doReg_clicked()
{
    QString login = ui->loginField->text().simplified();
    QString pass = ui->passField->text().simplified();
    QString pass2 = ui->passField2->text().simplified();//ОБРАБОТАТЬ ОШИБКИ, ВЫВЕСТИ ПОЛЬЗОВАТЕЛЮ!!! TODO
    if(login.length()<4 || login.length()>15){ error("Логин должен быть от 4 до 15 символов.");return;}
    if(pass.length()<6){ error("Слишком короткий пароль.");return;}
    if(pass != pass2){ error("Пароли не совпадают.");return;}
    error("Загрузка...");

    connecter *regConnect = new connecter("auth.php");
    regConnect->doConnect("act=reg&login="+login+"&password="+mdparser::parse(pass));
    connect(regConnect, SIGNAL(ready(QNetworkReply*)), this, SLOT(replyReady(QNetworkReply*)));
}

void reg::replyReady(QNetworkReply *rep)
{
    QString rp = rep->readAll();
    qDebug() << rp;
    if(rp == "regComplete"){
        error("Регистрация завершена!");}else{
        error("error: "+rp);
    }
}
