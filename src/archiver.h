#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <private/qzipreader_p.h>
#include <private/qzipwriter_p.h>
#include <QString>

class archiver
{
public:
    archiver(QString p);
    void unpack(QString outP);
private:
    QString path;
    QString outPath;
};

#endif // ARCHIVER_H
