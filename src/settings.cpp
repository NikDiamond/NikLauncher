#include "settings.h"

settings::settings()
{
    clientVer = "0.01";


    //domain = "dibgenerator.hol.es";
    //pathOnSite = "NikLauncher";

    domain = "nikdiamond.hol.es";
    pathOnSite = "NikLauncher";

    launcherName = "NikLauncher";
    serversList[0] << "Vanilla" << "nikd.zapto.org" << "25565" << "1.8";
    globalPath = QStandardPaths::standardLocations(QStandardPaths::RuntimeLocation).at(0) + "/." + launcherName + "/";
    javaPath = "C:/Program Files/Java/jre7/bin/javaw.exe";
}
