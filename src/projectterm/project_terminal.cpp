#include "config/appl_conf.h"
#include "config/logger_conf.h"
#include "shared/logger/logger.h"
#include "shared/logger/format.h"
#include "logger/config.h"

#include "pproto/transport/tcp.h"
#include "pproto/transport/local.h"

#include <QDir>
#include <QString>

#include <string>

#define log_error_m   alog::logger().error   (alog_line_location, "MAIN")
#define log_warn_m    alog::logger().warn    (alog_line_location, "MAIN")
#define log_info_m    alog::logger().info    (alog_line_location, "MAIN")
#define log_verbose_m alog::logger().verbose (alog_line_location, "MAIN")
#define log_debug_m   alog::logger().debug   (alog_line_location, "MAIN")
#define log_debug2_m  alog::logger().debug2  (alog_line_location, "MAIN")

using namespace std;
using namespace pproto;
using namespace pproto::transport;

int main (int argc, char* argv[])
{
    alog::logger().start();
    alog::logger().addSaverStdOut(alog::Level::Debug2);


//    config::base().setReadOnly(true);
//    config::base().setSaveDisabled(true);

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

    return 0;
}
