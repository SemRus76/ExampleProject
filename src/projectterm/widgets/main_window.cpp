#include "main_window.h"
#include "ui_main_window.h"

using namespace pproto;
using namespace pproto::transport;
using namespace pproto::transport::tcp;
using namespace pproto::command;

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
    FUNC_REGISTRATION(ChatMessage)
    FUNC_REGISTRATION(ChatHistory)

    #undef FUNC_REGISTRATION

    ui->btnSendMessage->setDisabled(true);
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

        if (_serverSocket->isConnected())
        {
            pproto::data::ChatHistory request;
            pproto::Message::Ptr message = createMessage(request);
            message->setType(pproto::Message::Type::Command);
            _serverSocket->send(message);
        }
    }
}

void MainWindow::command_ChatMessage(const pproto::Message::Ptr& message)
{
    pproto::data::ChatMessage response;
    readFromMessage(message, response);

    QString line = "[";
    line += response.date.toString("dd.MM.yyyy hh:mm:ss");
    line += "] ";
    line += response.nickname;
    line += " : ";
    line += response.text;

    ui->chat->append(line);
}

void MainWindow::command_ChatHistory(const pproto::Message::Ptr& message)
{
    if (message->type() == Message::Type::Answer)
    {
        pproto::data::ChatHistory response;
        readFromMessage(message, response);

        for (int i = 0; i < response.history.size(); ++i)
        {
            QString line = "[";
            line += response.history[i]->date.toString("dd.MM.yyyy hh:mm:ss");
            line += "] ";
            line += response.history[i]->nickname;
            line += " : ";
            line += response.history[i]->text;

            ui->chat->append(line);
        }
        ui->btnSendMessage->setDisabled(false);
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


void MainWindow::on_btnSendMessage_clicked()
{
    if (!_serverSocket->isConnected())
        // TODO По-хорошему, нужно сообщить ползователю, что связь потеряна
        return;

    if (ui->nick->text().isEmpty())
        // TODO По-хорошему, нужно сообщить ползователю, что-бы он написал ник
        return;

    if (ui->message->text().isEmpty())
        // TODO По-хорошему, нужно сообщить ползователю, что-бы он написал сообщение
        return;

    pproto::data::ChatMessage chatMessage;

    chatMessage.date = QDateTime::currentDateTime();
    chatMessage.nickname = ui->nick->text();
    chatMessage.text = ui->message->text();

    pproto::Message::Ptr message = createMessage(chatMessage);
    message->setType(pproto::Message::Type::Command);

    _serverSocket->send(message);
    ui->message->clear();
}

