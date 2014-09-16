#include "archiver.h"

archiver::archiver(QString p):path(p)
{}

void archiver::unpack(QString outP)
{
    outPath = outP;
    QZipReader *reader = new QZipReader(path);
    reader->extractAll(outP);
}

