#include "application.h"


#define log_error_m   alog::logger().error   (alog_line_location, "Application")
#define log_warn_m    alog::logger().warn    (alog_line_location, "Application")
#define log_info_m    alog::logger().info    (alog_line_location, "Application")
#define log_verbose_m alog::logger().verbose (alog_line_location, "Application")
#define log_debug_m   alog::logger().debug   (alog_line_location, "Application")
#define log_debug2_m  alog::logger().debug2  (alog_line_location, "Application")

Application::Application(int argc, char** argv) :
    QCoreApplication(argc, argv)
{}

bool Application::init()
{
    QUuidEx idAppl = QUuidEx::createUuid();
    if (!config::base().getValue("application.id", idAppl))
    {
        log_error_m << "Error init application - " << pproto::error::init_appl_id.description;
        return false;
    }
    _idAppl = idAppl;

    QString nameAppl;
    if (!config::base().getValue("application.name", nameAppl))
    {
        log_error_m << "Error init application - " << pproto::error::init_appl_name.description;
        return false;
    }
    _nameAppl = nameAppl;

    QString pathSaver;
    if (!config::base().getValue("logger.file", pathSaver))
    {
        log_error_m << "Error init application - " << pproto::error::init_appl_saverPath.description;
        return false;
    }

    #define FUNC_REGISTRATION(COMMAND) \
        _funcInvoker.registration(command:: COMMAND, &Application::command_##COMMAND, this);

    FUNC_REGISTRATION(ServerInformation)

    #undef FUNC_REGISTRATION

    log_info_m << "Application is Creating: " << _idAppl.toString() << " - " << _nameAppl;
    return true;
}

void Application::deInit()
{

}

void Application::socketConnected(pproto::SocketDescriptor socketDescript)
{
//    data::ServerInformation serverInformation;
//    Message::Ptr m = createMessage(serverInformation);
//    m->appendDestinationSocket(socketDescript);
//    transport::tcp::listener().send(m);
}

void Application::message(const pproto::Message::Ptr& message)
{
    // Не обрабатываем сообщения если приложение получило команду на остановку
    if (_stop)
        return;

    if (message->processed())
        return;

    if (lst::FindResult fr = _funcInvoker.findCommand(message->command()))
    {
        if (command::pool().commandIsSinglproc(message->command()))
            message->markAsProcessed();
        _funcInvoker.call(message, fr);
    }
}

void Application::command_ServerInformation(const Message::Ptr& message)
{
    if (message->type() == Message::Type::Command)
    {
        data::ServerInformation serverInformation;
        readFromMessage(message, serverInformation);

        serverInformation.name = _nameAppl;

        QHostAddress addr {"127.0.0.1"};
        config::readHostAddress("listener.socket.address", addr);
        serverInformation.address = addr.toString();

        Message::Ptr answer = message->cloneForAnswer();
        writeToMessage(serverInformation, answer);
        transport::tcp::listener().send(answer);
    }
}







