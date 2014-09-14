#ifndef MDPARSER_H
#define MDPARSER_H

#include <QString>
#include <QCryptographicHash>

class mdparser
{
public:
    mdparser();
    static QString parse(QString str);
};

#endif // MDPARSER_H
