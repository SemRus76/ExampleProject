#include <iostream>
#include <QString>
#include <QStringList>
#include <QNetworkProxy>
#include <QObject>

#include "config/appl_conf.h"
#include "config/logger_conf.h"
#include "shared/logger/logger.h"
#include "shared/logger/format.h"
#include "logger/config.h"

#include "pproto/transport/tcp.h"
#include "pproto/transport/local.h"

#include "application.h"

#define log_error_m   alog::logger().error   (alog_line_location, "MAIN")
#define log_warn_m    alog::logger().warn    (alog_line_location, "MAIN")
#define log_info_m    alog::logger().info    (alog_line_location, "MAIN")
#define log_verbose_m alog::logger().verbose (alog_line_location, "MAIN")
#define log_debug_m   alog::logger().debug   (alog_line_location, "MAIN")
#define log_debug2_m  alog::logger().debug2  (alog_line_location, "MAIN")

/*
 * Домашнее задание - Необходимо реализовать простой чат, с возможностью подключения
 * ЛЮБОГО кол-ва участников и возможностью общения клиентов между ними.
 * Каждый подключившийся характеризуется своим никнеймом (без указанного никнейма
 * не давать клиенту подключаться к серверу).
 *
 * При отправке сообщения любым клиентом, сервер, помимо рассылки этого сообщения
 * всем остальным клиентам, должен написать в лог сообщение следующего содержания:
 * Никнейм клиента - Текст Сообщения
*/

// info - Общий уровень логированияё, чаще всего предполагае вывод только информационных и error сообщений
// debug - Уровень логирования разработки, который позволяет отследить поведение модулей системы на ходу
// debug2 - То же что и debug, но подробнее

void stopProgram()
{
    #define STOP_THREAD(THREAD_FUNC, NAME, TIMEOUT) \
        if (!THREAD_FUNC.stop(TIMEOUT * 1000)) { \
            log_info << "Thread '" NAME "': Timeout expired, thread will be terminated"; \
            THREAD_FUNC.terminate(); \
        }


//    STOP_THREAD(pproto::transport::tcp::listener(), "TransportSocket", 15)

    #undef STOP_THREAD

    pproto::transport::tcp::listener().close();
    config::observerBase().stop();

    alog::logger().flush();
    alog::logger().waitingFlush();

    alog::stop();
}

//using namespace pproto;
using namespace pproto::transport;

using namespace std;
int main(int argc, char* argv[])
{

    alog::logger().start();
    alog::logger().addSaverStdOut(alog::Level::Debug2);

    QString configFile = config::qdir() + "/example.conf";
    config::dirExpansion(configFile);
    if (!QFile::exists(configFile))
    {
        log_error_m << "Config file " << configFile.toStdString() << " not exists";
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

    config::observerBase().start();

    alog::logger().start();

    alog::configDefaultSaver();
    alog::configExtendedSavers();
    alog::printSaversInfo();

    alog::logger().removeSaverStdOut();
    alog::logger().removeSaverStdErr();
    alog::logger().addSaverStdOut(alog::Level::Info);

    if (!pproto::command::checkUnique())
    {
        stopProgram();
        return 1;
    }

    if (!pproto::error::checkUnique())
    {
        stopProgram();
        return 1;
    }

    // Создание объекта приложения
    Application appl {argc, argv};

    QNetworkProxy::setApplicationProxy(QNetworkProxy::NoProxy);

    // Запуск всех потоков обслуживания приложения
    QHostAddress address {"0.0.0.0"};
    config::readHostAddress("listener.socket.address", address);

    int port {20102};
    config::base().getValue("listener.socket.port", port);

    if (!pproto::transport::tcp::listener().init({address, port}))
    {
        log_error_m << "Error init socket. Close program";
        stopProgram();
        return 1;
    }

    // Инициализация объекта приложения
    if (!appl.init())
    {
        log_error_m << "Error start Program. Close program";
        stopProgram();
        return 1;
    }

    chk_connect_q(&tcp::listener(), &tcp::Listener::message,
                  &appl           , &Application::message  )

    chk_connect_q(&tcp::listener(), &tcp::Listener::socketConnected,
                  &appl           , &Application::socketConnected)

    appl.exec();

    stopProgram();
    return 0;
}
