#include "chatserver.h"
#include <iostream>
using namespace std;

ChatServer::ChatServer(int port, boost::mutex &mutex) {
    this->port = port;
    this->mutex = &mutex;
    // Set logging settings
    server.set_access_channels(websocketpp::log::alevel::all);
    server.clear_access_channels(websocketpp::log::alevel::frame_payload);

    // Initialize Asio
    server.init_asio();

    // Register Event Handlers
    server.set_open_handler(bind(&ChatServer::on_successful_new_connection, this, placeholders::_1));
    server.set_close_handler(bind(&ChatServer::on_close_connection, this, placeholders::_1));
    server.set_fail_handler(bind(&ChatServer::on_connection_failed, this, placeholders::_1));
    server.set_message_handler(bind(&ChatServer::on_message, this, placeholders::_1, placeholders::_2));
}

void ChatServer::start() {
    log("Starting server on ws://localhost:" + to_string(port));
    server.listen(port);
    server.start_accept();

    server.run();

}
void ChatServer::stop() {
    log("Stopping server!");
    server.stop_listening();
    for (const auto& conn : connection_list){
        server.close(conn, websocketpp::close::status::going_away, "Server is shutting down.");
    }
}
void ChatServer::log(const std::string& message) {
    server.get_alog().write(websocketpp::log::alevel::app, message);
}

int ChatServer::get_total_connections() const {
    return 0;
}

void ChatServer::send_message(const string &msg) {

}

void ChatServer::broadcast_message(const string &msg) {
    log("[broadcast_message] " + msg);
    for (const auto& conn : connection_list){
        server.send(conn, msg, websocketpp::frame::opcode::text);
    }
}

void ChatServer::on_successful_new_connection(const Connection& connection) {
    connection_list.insert(connection);
    string msg = "A user joined the chatroom. Total online users: " + to_string(connection_list.size());
    broadcast_message(msg);
}

void ChatServer::on_connection_failed(const Connection& connection) {
    string msg = "Connection failed for client"; //TODO: log ip
    log(msg);
}

void ChatServer::on_close_connection(const Connection& connection) {
    connection_list.erase(connection);
}

void ChatServer::on_message(const Connection& connection, const Message& message) {
    log(message->get_payload());
}

