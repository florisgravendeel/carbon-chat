#include "chatserver.h"

ChatServer::ChatServer(int port) {
    ChatServer::port = port;
    uri = "ws://localhost:" + std::to_string(port);
    server.init_asio();

    server.set_access_channels(websocketpp::log::alevel::all);
    server.clear_access_channels(websocketpp::log::alevel::frame_payload);

    using websocketpp::lib::bind;
    using websocketpp::lib::placeholders::_1;
    using websocketpp::lib::placeholders::_2;

    server.set_open_handler(bind(&ChatServer::on_successful_new_connection, this, _1));
    server.set_close_handler(bind(&ChatServer::on_close_connection, this, _1));
    server.set_fail_handler(bind(&ChatServer::on_connection_failed, this, _1));
    server.set_message_handler(bind(&ChatServer::on_message_received, this, _1, _2));
}

void ChatServer::start() {
    websocketpp::lib::thread command_prompt_thread(&ChatServer::open_command_prompt, this);
    websocketpp::lib::thread server_thread(&Server::run, &server);
    server.listen(port);
    server.start_accept();
    server.run();
    server_thread.join();
    command_prompt_thread.join();
}

void ChatServer::stop() {
    server.stop_listening();
    for (auto it: connections) {
        websocketpp::lib::error_code error;
        server.close(it, websocketpp::close::status::going_away, "Server shutting down.", error);
        if (error) {
            std::cout << "Error stop(): " << error.message() << std::endl;
        }
    }
    server.stop();
}

void ChatServer::send_message(Connection connection, const std::string &msg) {

}

void ChatServer::broadcast_message(const std::string &msg) {

}

void ChatServer::on_successful_new_connection(Connection connection) {
    connections.insert(connection);
}

void ChatServer::on_connection_failed(Connection connection) {
     std::cout << "Connection failed for client." << std::endl; // TODO: use log
}

void ChatServer::on_close_connection(Connection connection) {
    connections.erase(connection);
}

void ChatServer::on_message_received(Connection connection, Message message) {
    if (message->get_payload() == "User: stopserver") {
        stop();
        return;
    }
    for (auto it: connections) {
        server.send(it, message);
    }
}

void ChatServer::log(const std::string &message) {

}

void ChatServer::open_command_prompt() {
    std::string input;
    while (true) {
        //Read user input from stdin
        std::getline(std::cin, input);
        if (input == "/stop") {
            stop();
            break;
        }
    }
}
//#include <set>
//#include <websocketpp/config/asio_no_tls.hpp>
//#include <websocketpp/server.hpp>
//#include <websocketpp/common/thread.hpp>

//typedef websocketpp::server<websocketpp::config::asio> server;
//using websocketpp::connection_hdl;

//class Chatserver {
//public:

//

//
//

//

//

//private:
//    typedef std::set<connection_hdl,std::owner_less<connection_hdl>> con_list;
//
//    server m_server;
//    con_list m_connections;
//};
