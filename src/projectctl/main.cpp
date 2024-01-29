#include <iostream>
#include <QString>
#include <QStringList>

#include "config/appl_conf.h"
#include "config/logger_conf.h"
#include "shared/logger/logger.h"
#include "shared/logger/format.h"
#include "logger/config.h"

using namespace std;
int main(int argc, char* argv[])
{

    Q_UNUSED(argv)

    alog::logger().start();
    alog::logger().addSaverStdOut(alog::Level::Debug2);

    if (argc > 0)
    {
//        QStringList strList;
//        for (int i = 0; i < argc; ++i)
//            strList.append(argv[i]);

//        for (QString& elem : strList)
//            cout << "Paramentr " << elem.toStdString() << endl;
    }

    QString configFile = config::qdir() + "/example.conf";

    config::dirExpansion(configFile);
    if (!QFile::exists(configFile))
    {
//        log_error << "Config file " << configFile << " not exists";
        alog::stop();
        return 1;
    }

    config::base().setReadOnly(true);
    config::base().setSaveDisabled(true);
    if (!config::base().readFile(configFile.toStdString()))
    {
        alog::stop();
        return 1;
    }

    alog::logger().start();

//    alog::configExtendedSavers();
    alog::configDefaultSaver();
    alog::printSaversInfo();

    alog::logger().removeSaverStdOut();
    alog::logger().removeSaverStdErr();
//    alog::logger().addSaverStdOut(alog::Level::Info);

    QString idAppl = QString();
    config::base().getValue("application.id", idAppl); // Нельзя не инициализированные переменные

    QString nameAppl = QString();
    config::base().getValue("application.name", nameAppl);

    QString pathSaver = QString();
    config::base().getValue("logger.file", pathSaver);

    log_info << "Hello World " << idAppl.toStdString() << " - " << nameAppl.toStdString();
    log_info << "Logger file: " << pathSaver.toStdString();

    for (int i = 0;; ++i)
    {
        log_info << "Hello - " << i;
        this_thread::sleep_for(chrono::seconds(1));
    }

    return 0;
}
