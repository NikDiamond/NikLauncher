#ifndef AUTHUTIL_H
#define AUTHUTIL_H

#include <QString>

class authUtil
{
public:
    authUtil();
    ~authUtil();
    void auth(QString login, QString pass);
};

#endif // AUTHUTIL_H
