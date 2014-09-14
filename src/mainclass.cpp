#include "mainclass.h"
#include "connecter.h"
#include "mdparser.h"
#include "ui_mainclass.h"
#include "reg.h"

MainClass::MainClass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainClass)
{
    connecter *getServersConnect = new connecter("servers.php");
    getServersConnect->doConnect();
    connect(getServersConnect, SIGNAL(ready(QNetworkReply*)), this, SLOT(serversWrite(QNetworkReply*)));

    ui->setupUi(this);
    setUi();
}

MainClass::~MainClass()
{delete ui;}

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
    if (mpos.x() >= 0 && event->buttons() && Qt::LeftButton)
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
    qDebug() << rpList;
}

void MainClass::on_toGame_clicked()
{
    QString login = ui->loginField->text().simplified();
    QString pass = ui->passField->text().simplified();
    if(login.length()<6 || login.length()>15){status("Неверный логин."); return;}
    if(pass.length()<6){status("Неверный пароль."); return;}

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
        status("Запуск игры...");
    }else{
        status(rp);
    }
}

void MainClass::on_toReg_clicked()
{
    reg *regForm = new reg(this);
    regForm->setGeometry(-1,49,0,0);
    regForm->setMaximumSize(width()+1,height()-49);
    regForm->setMinimumSize(width()+1,height()-49);
    regForm->show();
    regForm->setUi();
}

void MainClass::on_toClose_clicked()
{
    close();
}
