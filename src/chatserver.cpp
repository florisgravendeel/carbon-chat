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
    server.set_open_handler(bind(&ChatServer::on_successful_new_connection, this, std::placeholders::_1));
    server.set_close_handler(bind(&ChatServer::on_close_connection, this, std::placeholders::_1));
    server.set_message_handler(bind(&ChatServer::on_message, this, std::placeholders::_1, std::placeholders::_2));
}

void ChatServer::start() {
    std::cout<<"Starting server on ws://localhost:"<< port;
    std::cout<<std::endl;
    server.listen(port);
    server.start_accept();
    server.run();

}
void ChatServer::stop() {
    std::cout <<  "Stopping server!" << std::endl;
    server.stop_listening();
//    server.close() Close all connections

}

int ChatServer::get_total_connections() const {
    return 0;
}

void ChatServer::send_message(const std::string &msg) {

}

void ChatServer::broadcast_message(const std::string &msg) {

}

void ChatServer::on_successful_new_connection(Connection connection) {

}

void ChatServer::on_connection_failed(Connection connection) {

}

void ChatServer::on_close_connection(Connection connection) {

}

void ChatServer::on_message(const Connection& connection, Message message) {
    std::cout << "Message received on connection(" << connection.lock().get() << "): " + message->get_payload() << std::endl;

}

