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
    permissions_key = " permissions_key: " + generate_permissions_key();
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
    log("Generated" + permissions_key);
    log("Starting server on " + uri);
    server.listen(port);
    server.start_accept();
    server.run();
}

void ChatServer::stop() {
    broadcast_message(SERVER_GOING_OFFLINE_MSG);

    initiating_shutdown = true; // See -> on_close_connection
    log("Total connections: " + std::to_string(connections.size()));

    // Close all existing connections.
    for (const auto& connection: connections) {
        ErrorCode error;

        server.close(connection, websocketpp::close::status::going_away, SERVER_GOING_OFFLINE_MSG, error);
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
    std::string msg = message->get_payload();
    log("[on_message_received] " + msg);
    if (msg == STOP_COMMAND + permissions_key){
        stop();
        return;
    } else if (msg.rfind(SHOW_JOIN_MSG_COMMAND, 0) == 0) { // Check for "/show-join-msg" (not -> <user>)
        std::string username = boost::replace_first_copy(msg, SHOW_JOIN_MSG_COMMAND " ", "");
        std::string join_msg = JOIN_MSG_FORMAT;
        boost::replace_all(join_msg, "%user", username);
        boost::replace_all(join_msg, "%count", std::to_string(connections.size()));
        broadcast_message(join_msg);
    } else {
        broadcast_message(message->get_payload());
    }
}

void ChatServer::log(const std::string &message) {
    server.get_alog().write(LogLevel::app, message);
}

std::string ChatServer::get_permissions_key() {
    return ChatServer::permissions_key;
}

std::string ChatServer::generate_permissions_key() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    return to_string(uuid);
}

