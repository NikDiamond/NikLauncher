#include "minecraft.h"

#include <QProcess>

minecraft::minecraft(QString sc, QString login, QString pass, QString access, QString server, QString port, QObject *parent) :
    QObject(parent),serverChoosen(sc),login(login),password(pass),accessToken(access),serverIp(server),serverPort(port)
{
    qDebug() << serverChoosen << login << password << accessToken << serverIp << serverPort;
}

void minecraft::launch()
{
    proc = new QProcess(this);
    QStringList args;

    //Java аргументы
    args << "-XX:HeapDumpPath=MojangTricksIntelDriversForPerformance_javaw.exe_minecraft.exe.heapdump";
    args << "-Xmx1G";
    args << "-XX:+UseConcMarkSweepGC";
    args << "-XX:+CMSIncrementalMode";
    args << "-XX:-UseAdaptiveSizePolicy";
    args << "-Xmn128M";

    //Указываем пути библиотек

    QString absPath = sett.globalPath + serverChoosen;
    args << "-Djava.library.path="+ absPath +"/bin/natives";
    args << "-cp" << absPath+"/bin/libraries/java3d/vecmath/1.5.2/vecmath-1.5.2.jar;/bin/libraries/net/sf/trove4j/trove4j/3.0.3/trove4j-3.0.3.jar;"+absPath+"/bin/libraries/com/ibm/icu/icu4j-core-mojang/51.2/icu4j-core-mojang-51.2.jar;"+absPath+"/bin/libraries/net/sf/jopt-simple/jopt-simple/4.6/jopt-simple-4.6.jar;"+absPath+"/bin/libraries/com/paulscode/codecjorbis/20101023/codecjorbis-20101023.jar;"+absPath+"/bin/libraries/com/paulscode/codecwav/20101023/codecwav-20101023.jar;"+absPath+"/bin/libraries/com/paulscode/libraryjavasound/20101123/libraryjavasound-20101123.jar;"+absPath+"/bin/libraries/com/paulscode/librarylwjglopenal/20100824/librarylwjglopenal-20100824.jar;"+absPath+"/bin/libraries/com/paulscode/soundsystem/20120107/soundsystem-20120107.jar;"+absPath+"/bin/libraries/io/netty/netty-all/4.0.15.Final/netty-all-4.0.15.Final.jar;"+absPath+"/bin/libraries/com/google/guava/guava/17.0/guava-17.0.jar;"+absPath+"/bin/libraries/org/apache/commons/commons-lang3/3.3.2/commons-lang3-3.3.2.jar;"+absPath+"/bin/libraries/commons-io/commons-io/2.4/commons-io-2.4.jar;"+absPath+"/bin/libraries/commons-codec/commons-codec/1.9/commons-codec-1.9.jar;"+absPath+"/bin/libraries/net/java/jinput/jinput/2.0.5/jinput-2.0.5.jar;"+absPath+"/bin/libraries/net/java/jutils/jutils/1.0.0/jutils-1.0.0.jar;"+absPath+"/bin/libraries/com/google/code/gson/gson/2.2.4/gson-2.2.4.jar;"+absPath+"/bin/libraries/com/mojang/authlib/1.5.17/authlib-1.5.17.jar;"+absPath+"/bin/libraries/com/mojang/realms/1.5.4/realms-1.5.4.jar;"+absPath+"/bin/libraries/org/apache/commons/commons-compress/1.8.1/commons-compress-1.8.1.jar;"+absPath+"/bin/libraries/org/apache/httpcomponents/httpclient/4.3.3/httpclient-4.3.3.jar;"+absPath+"/bin/libraries/commons-logging/commons-logging/1.1.3/commons-logging-1.1.3.jar;"+absPath+"/bin/libraries/org/apache/httpcomponents/httpcore/4.3.2/httpcore-4.3.2.jar;"+absPath+"/bin/libraries/org/apache/logging/log4j/log4j-api/2.0-beta9/log4j-api-2.0-beta9.jar;"+absPath+"/bin/libraries/org/apache/logging/log4j/log4j-core/2.0-beta9/log4j-core-2.0-beta9.jar;"+absPath+"/bin/libraries/org/lwjgl/lwjgl/lwjgl/2.9.1/lwjgl-2.9.1.jar;"+absPath+"/bin/libraries/org/lwjgl/lwjgl/lwjgl_util/2.9.1/lwjgl_util-2.9.1.jar;"+absPath+"/bin/libraries/tv/twitch/twitch/6.5/twitch-6.5.jar;"+absPath+"/bin/minecraft.jar";

    //Вызываем главный класс
    args << "net.minecraft.client.main.Main";

    //Передаём аргументы в Minecraft
    args << "--username" << login;
    args << "--version" << "1.8";
    args << "--gameDir" << absPath+"/";
    args << "--assetsDir" << absPath+"/bin/assets/";
    args << "--assetIndex" << "1.8";
    args << "--uuid" << "";
    args << "--accessToken" << accessToken;
    args << "--userProperties" << "{}";
    args << "--userType" << "legacy";
    args << "--server" << serverIp;
    args << "--port" << serverPort;

    proc->start("C:/Program Files/Java/jre7/bin/javaw.exe",  args);
}
