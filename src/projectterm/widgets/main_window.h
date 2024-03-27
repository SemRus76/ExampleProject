#pragma once
#include "commands/commands.h"
#include "commands/error.h"

#include "shared/simple_ptr.h"
#include "shared/defmac.h"
#include "shared/logger/logger.h"
#include "shared/logger/format.h"
#include "shared/config/appl_conf.h"
#include "shared/qt/logger_operators.h"

#include "pproto/func_invoker.h"
#include "pproto/transport/tcp.h"
#include "pproto/commands/base.h"
#include "pproto/commands/pool.h"
#include "pproto/logger_operators.h"

#include <QApplication>
#include <QMainWindow>
#include <QCloseEvent>
#include <QHostInfo>
#include <QMessageBox>
#include <QDateTime>

#include <unistd.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void message(const pproto::Message::Ptr&);

    void command_ServerInformation(const pproto::Message::Ptr&);
    void command_ChatMessage      (const pproto::Message::Ptr&);
    void command_ChatHistory      (const pproto::Message::Ptr&);

private slots:
    void on_btnConnectToServer_clicked();

    void on_btnSendMessage_clicked();

private:
    Ui::MainWindow *ui;
    pproto::FunctionInvoker _funcInvoker;

    pproto::transport::tcp::Socket::Ptr _serverSocket;
};

