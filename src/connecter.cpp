#include "connecter.h"

connecter::connecter(QString u, QObject *parent) :
    QObject(parent),
    url(u)
{}

connecter::~connecter(){}

void connecter::doConnect(QString params){

    manager = new QNetworkAccessManager;

    QNetworkRequest *request = new QNetworkRequest(QUrl("http://"+sett.domain+"/"+sett.pathOnSite+"/"+url));
    qDebug() << "Connecting to " << "http://"+sett.domain+"/"+sett.pathOnSite+"/"+url << " ...";
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request->setHeader(QNetworkRequest::UserAgentHeader, sett.clientVer);

    manager->post(*request, params.toUtf8());
    connect(manager, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(gotRequest(QNetworkReply*)));
}

void connecter::gotRequest(QNetworkReply *rep){
    emit ready(rep);
}
