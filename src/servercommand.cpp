//
// Created by Floris Gravendeel on 24/01/2023.
//
#include "servercommand.h"

ServerCommand::ServerCommand(int port, const std::string& command, const std::string& permissions_key) {
    ServerCommand::command = command;
    ServerCommand::permissions_key = permissions_key;
    ServerCommand::command_executed = false;
    server_uri = "ws://localhost:" + std::to_string(port);

    // Disable logging completely
    client.clear_access_channels(LogLevel::none);
    client.set_access_channels(LogLevel::none);

    // Documentation: Initialize asio transport with internal io_service
    client.init_asio();

    using websocketpp::lib::placeholders::_1;
    using websocketpp::lib::bind;
    client.set_open_handler(bind(&ServerCommand::on_successful_new_connection, this, _1));
}

void ServerCommand::on_successful_new_connection(const Connection &connection) {
    client.send(connection, command + permissions_key,
                websocketpp::frame::opcode::text);
}

void ServerCommand::execute() {

    ErrorCode error;
    Client::connection_ptr con = client.get_connection(server_uri, error);
    if (error) {
        client.get_alog().write(LogLevel::app, "Connection error: " + error.message());
        return;
    }

    // Queue the connection. No network connections will be made until the io_service is running.
    client.connect(con);

    // Create a thread to run the asio io_service.
    Thread asio_thread(&Client::run, &client);
    asio_thread.join();
}

void ServerCommand::disconnect(const Connection &connection) {
    client.close(connection, websocketpp::close::status::normal, "Command executed");
}

