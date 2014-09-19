#ifndef REG_H
#define REG_H

#include <QWidget>
#include <QNetworkReply>

namespace Ui {
class reg;
}

class reg : public QWidget
{
    Q_OBJECT

public:
    explicit reg(QWidget *parent = 0);
    ~reg();
    void setUi();
    void error(QString err);

private slots:
    void on_toBack_clicked();

    void on_doReg_clicked();

private:
    Ui::reg *ui;
private slots:
    void replyReady(QNetworkReply *rep);
signals:
    void closed();
};

#endif // REG_H
