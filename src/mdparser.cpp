#include "mdparser.h"

mdparser::mdparser()
{}

QString mdparser::parse(QString str)
{
    QCryptographicHash md5Generator(QCryptographicHash::Md5);
    md5Generator.addData(str.toUtf8());
    return md5Generator.result().toHex();
}
