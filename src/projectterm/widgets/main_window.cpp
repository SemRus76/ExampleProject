#include "main_window.h"
#include "ui_main_window.h"

using namespace pproto;
using namespace pproto::transport;
using namespace pproto::transport::tcp;

#define log_error_m   alog::logger().error   (alog_line_location, "Main_Window")
#define log_warn_m    alog::logger().warn    (alog_line_location, "Main_Window")
#define log_info_m    alog::logger().info    (alog_line_location, "Main_Window")
#define log_verbose_m alog::logger().verbose (alog_line_location, "Main_Window")
#define log_debug_m   alog::logger().debug   (alog_line_location, "Main_Window")
#define log_debug2_m  alog::logger().debug2  (alog_line_location, "Main_Window")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _serverSocket = pproto::transport::tcp::Socket::Ptr {new Socket()};

    #define FUNC_REGISTRATION(COMMAND) \
        _funcInvoker.registration(command:: COMMAND, &MainWindow::command_##COMMAND, this);

    FUNC_REGISTRATION(ServerInformation)

    #undef FUNC_REGISTRATION
}

MainWindow::~MainWindow()
{
    if (_serverSocket)
        _serverSocket->disconnect();
    delete ui;
}

void MainWindow::message(const pproto::Message::Ptr& message)
{
    // Не обрабатываем сообщения если приложение получило команду на остановку
    if (message->processed())
        return;

    if (lst::FindResult fr = _funcInvoker.findCommand(message->command()))
    {
        if (command::pool().commandIsSinglproc(message->command()))
            message->markAsProcessed();
        _funcInvoker.call(message, fr);
    }
}

void MainWindow::command_ServerInformation(const pproto::Message::Ptr& message)
{
    if (message->type() == Message::Type::Answer)
    {
        data::ServerInformation serverInformation;
        readFromMessage(message, serverInformation);

        ui->nameServerInfo  ->setText(serverInformation.name   );
        ui->adressServerInfo->setText(serverInformation.address);
    }
}

void MainWindow::on_btnConnectToServer_clicked()
{   
    QString strAddress;
    config::base().getValue("listener.server.address", strAddress);
    QHostAddress address = QHostAddress(strAddress);

    int port = {0};
    config::base().getValue("listener.server.port", port);

    if (!_serverSocket->init({address,port}))
    {
        QMessageBox message;
        message.setText("Failed create Client Socket!");
        message.exec();
        log_error_m << "Failed create Client Socket";
        return;
    }
    _serverSocket->start();


    int attempts = 0;
    while (attempts++ < 60 /*ждем 6 сек*/)
    {
        _serverSocket->connect();
        qApp->processEvents();
        QThread::msleep(100 /*0.1 сек*/);
        if (_serverSocket->isConnected())
            break;
        _serverSocket->stop();
    }
    if (!_serverSocket->isConnected())
    {
        QMessageBox message;
        message.setText("Failed Connect to Server!");
        message.exec();
        log_error_m << "Failed Connect to Server";
        _serverSocket->stop();
        return;
    }

    chk_connect_q(_serverSocket , &tcp::Socket::message,
                  this          , &MainWindow ::message )

    pproto::data::ServerInformation request;

    request.HelloWorld = "Hello World";

    pproto::Message::Ptr msg = createMessage(request);
    msg->setType(pproto::Message::Type::Command);

    _serverSocket->send(msg);
    log_info_m << "Send request to Server Information";
}

