#include "config/appl_conf.h"
#include "config/logger_conf.h"
#include "shared/logger/logger.h"
#include "shared/logger/format.h"
#include "logger/config.h"

#include "pproto/transport/tcp.h"
#include "pproto/transport/local.h"

#include <QDir>
#include <QString>
#include <QApplication>
#include <QNetworkProxy>
#include "widgets/main_window.h"

#include <string>

#define log_error_m   alog::logger().error   (alog_line_location, "DESKTOP MAIN")
#define log_warn_m    alog::logger().warn    (alog_line_location, "DESKTOP MAIN")
#define log_info_m    alog::logger().info    (alog_line_location, "DESKTOP MAIN")
#define log_verbose_m alog::logger().verbose (alog_line_location, "DESKTOP MAIN")
#define log_debug_m   alog::logger().debug   (alog_line_location, "DESKTOP MAIN")
#define log_debug2_m  alog::logger().debug2  (alog_line_location, "DESKTOP MAIN")

using namespace std;
using namespace pproto;
using namespace pproto::transport;

int main (int argc, char* argv[])
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    alog::logger().start();
    alog::logger().addSaverStdOut(alog::Level::Debug2);

    string cfgDir {CONFIG_DIR_CLIENT};

    if (!QDir(QString::fromStdString(cfgDir)).exists())
        if (!QDir().mkpath(QString::fromStdString(cfgDir)))
        {
            log_error << "Failed create config directory: " << cfgDir;
            alog::stop();
            return 1;
        }

    QString configFile = QString::fromStdString(cfgDir);
    configFile += "/client.conf";
    config::dirExpansion(configFile);
    if (!QFile::exists(configFile))
    {
        log_error_m << "Config file " << configFile.toStdString() << " not exists";

        QFile file {":/config/client.conf"};

        QByteArray conf;
        file.open(QIODevice::ReadOnly);
        conf = file.readAll();

        if (!config::base().readString(conf.toStdString()))
        {
            alog::stop();
            return 1;
        }
        if (!config::base().saveFile(configFile.toStdString()))
        {
            alog::stop();
            return 1;
        }

    }
    else
        config::base().readFile(configFile.toStdString());

    config::observerBase().start();

    alog::logger().start();

    alog::configDefaultSaver();
    alog::configExtendedSavers();
    alog::printSaversInfo();

    alog::logger().removeSaverStdOut();
    alog::logger().removeSaverStdErr();
    alog::logger().addSaverStdOut(alog::Level::Info);

    QApplication application(argc, argv);

    application.setApplicationName(u8"Клиент Desktop");
    application.setApplicationVersion(u8"0.0.1");

    // Устанавливаем текущую директорию. Эта конструкция работает только
    // когда создан экземпляр QCoreApplication.
    if (QDir::setCurrent(QCoreApplication::applicationDirPath()))
    {
        log_debug << "Set work directory: " << QCoreApplication::applicationDirPath();
    }
    else
    {
        log_error << "Failed set work directory";
//        stopProgram();
        return 1;
    }

    if (!pproto::command::checkUnique())
    {
//        stopProgram();
        return 1;
    }

    if (!pproto::error::checkUnique())
    {
//        stopProgram();
        return 1;
    }

    QNetworkProxy::setApplicationProxy(QNetworkProxy::NoProxy);

    MainWindow clientWindow;
//    if (clientWindow.init())
//    {
//        log_error << "Failed init Main Window";
//        stopProgram();
//        return 1;
//    }

    clientWindow.show();
    int exitCode = application.exec();
    clientWindow.close();

    return exitCode;
}
