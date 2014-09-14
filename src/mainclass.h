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
private slots:
    void on_toGame_clicked();
    void startUserParsing(QNetworkReply *rep);

    void on_toReg_clicked();

    void on_toClose_clicked();

    void serversWrite(QNetworkReply *rep);
private:
    Ui::MainClass *ui;
    QPoint mpos;
    settings sett;
};

#endif // MAINCLASS_H
