#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QStandardPaths>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QByteArray>
#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QCryptographicHash>
#include <QComboBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void startGame();
    void validatePath();
    void auth();
    void registration();
    void log(QString msg);
    void log(QStringList msg);
    void getServers();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QProcess *proc;
    QNetworkAccessManager *manager;
    QNetworkReply *authReply;
    QNetworkReply *regReply;
    QNetworkReply *serversGotReply;

    QString userName;
    QString password;
    QStringList authData;
    QMap<int, QStringList> serversMap;

    bool isDebug;
    bool autoJoin;
    QString domain;
    QString clientVersion;
private slots:
    void on_startGame_clicked();
    void writeOut();
    void authGotReply();
    void regGotReply();
    void on_registration_clicked();
    void serversGot();
};

#endif // MAINWINDOW_H
