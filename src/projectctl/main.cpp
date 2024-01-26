#include <iostream>
#include <QString>
#include <QStringList>

#include "config/appl_conf.h"

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

    QString idAppl = QString();
    config::base().getValue("application.id", idAppl); // Нельзя не инициализированные переменные

    QString nameAppl = QString();
    config::base().getValue("application.name", nameAppl);

    cout << idAppl.toStdString() << " - " << nameAppl.toStdString() << endl;

    QUuidEx newId = QUuidEx::createUuid();
    config::base().setValue("application.id", newId);
//    config::base().saveFile(config::base().filePath());

    return 0;
}
