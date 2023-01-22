#include "chatserver.h"
using namespace std;

ChatServer::ChatServer(int port) {
    uri =  "ws://localhost:" + std::to_string(port);
    ChatServer::port = port;
    // Set logging settings to all except for frame level (too noisy)
    server.set_access_channels(websocketpp::log::alevel::all);
    server.clear_access_channels(websocketpp::log::alevel::frame_payload);

    // Initialize Asio
    server.init_asio();

    using websocketpp::lib::placeholders::_1;
    using websocketpp::lib::placeholders::_2;
    using websocketpp::lib::bind;

    // Register Event Handlers
    server.set_open_handler(bind(&ChatServer::on_successful_new_connection, this, _1));
    server.set_close_handler(bind(&ChatServer::on_close_connection, this, _1));
    server.set_fail_handler(bind(&ChatServer::on_connection_failed, this, _1));
    server.set_message_handler(bind(&ChatServer::on_message, this, _1, _2));
}

void ChatServer::start() {
    log("Starting server on " + uri);

    Thread command_prompt(&ChatServer::open_command_prompt, this);
    server.listen(port);
    server.start_accept();
    server.run();
    Thread asio_thread(&Server::run, &server);
    asio_thread.join();
    command_prompt.join();

}
void ChatServer::stop() {
    log("Stopping server!");
    server.stop_listening();

//    for (const auto& conn : connection_list){
//        log("X");
//        server.close(conn, websocketpp::close::status::going_away, "Server is shutting down.");
//    }
}
void ChatServer::log(const std::string& message) {
    server.get_alog().write(LogLevel::app, message);
}

int ChatServer::get_total_connections() const {
    return 0;
}

void ChatServer::send_message(const string &msg) {

}

void ChatServer::broadcast_message(const string &msg) {
    log("[broadcast_message] " + msg);
//    for (const auto& conn : connection_list){
//        server.send(conn, msg, websocketpp::frame::opcode::text);
//    }
}

void ChatServer::on_successful_new_connection(const Connection& connection) {
//    connection_list.insert(connection);
//    string client = server.get_connection()->get_remote_endpoint();
    log("New connection from! ");
//    string msg = "A user joined the chatroom. Total online users: " + to_string(connection_list.size());
//    broadcast_message("msg");
}

void ChatServer::on_connection_failed(const Connection& connection) {
//    string client = server.get_connection()->get_remote_endpoint();
    server.close(connection, websocketpp::frame::opcode::text, "Closing socket.");
    log("Connection failed for client!");
}

void ChatServer::on_close_connection(const Connection& connection) {

    log("Connection closed.");
//    connection_list.erase(connection);
}

void ChatServer::on_message(const Connection& connection, const Message& message) {
    log(message->get_payload());
}

void ChatServer::open_command_prompt() {
    string input;
    while (true)
    {
        //Read user input from stdin
        std::getline(std::cin, input);
        if (input == "/stop"){
            stop();
            break;
        }
    }
}