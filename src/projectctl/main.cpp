#include <iostream>
#include <QString>
#include <QStringList>

#include "config/appl_conf.h"
#include "config/logger_conf.h"
#include "shared/logger/logger.h"
#include "shared/logger/format.h"
#include "logger/config.h"

#define log_error_m   alog::logger().error   (alog_line_location, "Application")
#define log_warn_m    alog::logger().warn    (alog_line_location, "VideoFirst")
#define log_info_m    alog::logger().info    (alog_line_location, "Transport")
#define log_verbose_m alog::logger().verbose (alog_line_location, "Application")
#define log_debug_m   alog::logger().debug   (alog_line_location, "Application")
#define log_debug2_m  alog::logger().debug2  (alog_line_location, "Application")

// info - Общий уровень логирования, чаще всего предполагае вывод только информационных и error сообщений
// debug - Уровень логирования разработки, который позволяет отследить поведение модулей системы на ходу
// debug2 - То же что и debug, но подробнее

/*
 * Тренировка конфига и логгера
 *
 * Необходимо реализовать функцию вычесления ряда чисел Фибоначчи.
 * Номер последовательности ДО которого происходит вычесление задается в конфиге
 * При изменении конфига - должен запускаться процесс перерасчета
 *
 * Сам процесс расчета делится на 3 этапа: подготовительный, вычислительный и результирующий
 *
 * Этапы подготовки и результата выводятся в один лог файл уровнем info
 * Этап вычисления выводится во второй лог файл уровнем debug
 *
 * Все три этапа должны так же попадать в один общий лог с уровнем debug
 *
 * На этапе подготовки необходимо вывести сообщение о том какое число было получено из конфига
 * и какие числа берутся для позиций 0 и 1
 *
 * На этапе результата необходимо вывести сообщение, содержащее порядковый порядковый номер и
 * полученный результат.
 *
*/

using namespace std;
int main(int argc, char* argv[])
{
    Q_UNUSED(argv)
    Q_UNUSED(argc)

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

    QString idAppl = QString();
    config::base().getValue("application.id", idAppl); // Нельзя не инициализированные переменные

    QString nameAppl = QString();
    config::base().getValue("application.name", nameAppl);

    QString pathSaver = QString();
    config::base().getValue("logger.file", pathSaver);

    log_info_m << "Hello World " << idAppl.toStdString() << " - " << nameAppl.toStdString();
    log_info_m << "Logger file: " << pathSaver.toStdString();

    for (int i = 0;; ++i)
    {
        log_error_m << "Hello Application " << i;
        log_warn_m << "Hello Video " << i;
        log_info_m << "Hello Transport " << i;
        log_verbose_m << "Hello Scrum " << i;
        this_thread::sleep_for(chrono::seconds(1));
    }

    config::observerBase().stop();
    alog::logger().stop();

    return 0;
}
