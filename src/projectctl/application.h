#pragma once

#include "commands/commands.h"
#include "commands/error.h"

#include "pproto/commands/pool.h"
#include "pproto/func_invoker.h"

#include "config/appl_conf.h"
#include "shared/qt/quuidex.h"

#include <QCoreApplication>
#include <QObject>

#include <atomic>

using namespace std;
using namespace pproto;
class Application : public QCoreApplication
{
    Q_OBJECT
public:
    Application(int argc, char** argv);

    bool init();
    void deInit();

public slots:

    void message(const pproto::Message::Ptr&);

private:

    QUuidEx _idAppl;
    QString _nameAppl;

    atomic_bool _stop = {false};
    FunctionInvoker _funcInvoker;

};

