#include "authutil.h"

authUtil::authUtil()
{}

authUtil::~authUtil(){}

void authUtil::auth(QString login, QString pass){
    qDebug() << login << pass;
}
