//
// Created by Floris Gravendeel on 17/01/2023.
//
#define STOP_COMMAND "/stop"
#include "chatserver.h"
#include "servercommand.h"

ChatServer::ChatServer(int port) {
    ChatServer::port = port;
    uri = "ws://localhost:" + std::to_string(port);
    initiating_shutdown = false;

    // Initialize Asio
    server.init_asio();

    // Set logging settings to all except for frame level (too noisy)
    server.set_access_channels(LogLevel::app);
    server.clear_access_channels(LogLevel::frame_payload);

    using websocketpp::lib::bind;
    using websocketpp::lib::placeholders::_1;
    using websocketpp::lib::placeholders::_2;

    // Register Event Handlers
    server.set_open_handler(bind(&ChatServer::on_successful_new_connection, this, _1));
    server.set_close_handler(bind(&ChatServer::on_close_connection, this, _1));
    server.set_fail_handler(bind(&ChatServer::on_connection_failed, this, _1));
    server.set_message_handler(bind(&ChatServer::on_message_received, this, _1, _2));
}

void ChatServer::start() {
    log("Starting server on " + uri);
    server.listen(port);
    server.start_accept();
    server.run();
}

void ChatServer::stop() {
    log("Server shutting down");

    initiating_shutdown = true; // See -> on_close_connection
    log("Total connections: " + std::to_string(connections.size()));

    // Close all existing connections.
    for (const auto& connection: connections) {
        ErrorCode error;

        server.close(connection, websocketpp::close::status::going_away, "Server shutting down.", error);
        if (error) {
            std::cout << "Error stop(): " << error.message() << std::endl;
        }
    }
}

void ChatServer::send_message(Connection connection, const std::string &msg) {
    ErrorCode error;
    server.send(std::move(connection), msg, websocketpp::frame::opcode::text);
    if (error){
        log("Error sending message: " + error.message());
    }
}

void ChatServer::broadcast_message(const std::string &msg) {
    log("[broadcast_message] " + msg);
    for (const auto& connection : connections){
        send_message(connection, msg);
    }
}

void ChatServer::on_successful_new_connection(const Connection& connection) {
    log("New successful connection for client!");
    connections.insert(connection);
    std::string msg = "A user joined the chatroom. Total online users: " + std::to_string(connections.size());
    broadcast_message(msg);
}

void ChatServer::on_connection_failed(Connection connection) {
    Server::connection_ptr conn = server.get_con_from_hdl(std::move(connection));
    log("Connection failed for client: " + conn->get_ec().message());

}

void ChatServer::on_close_connection(const Connection& connection) {
    log("Closed connection for client.");
    connections.erase(connection);
    if (initiating_shutdown & connections.empty()) {
        log("All connections are closed.");
        server.stop();
    }
}

void ChatServer::on_message_received(const Connection& connection, const Message& message) {
    log("[on_message_received] " + message->get_payload());
    if (message->get_payload() == STOP_COMMAND){
        stop();
        return;
    }

    for (const auto& it: connections) {
        server.send(it, message);
    }
}

void ChatServer::log(const std::string &message) {
    server.get_alog().write(LogLevel::app, message);
}

