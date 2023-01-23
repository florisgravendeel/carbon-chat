#include "chatserver.h"

ChatServer::ChatServer(int port) {
    ChatServer::port = port;
    uri = "ws://localhost:" + std::to_string(port);

    // Initialize Asio
    server.init_asio();

    // Set logging settings to all except for frame level (too noisy)
    server.set_access_channels(LogLevel::all);
    server.clear_access_channels(LogLevel::frame_payload);

    using websocketpp::lib::bind;
    using websocketpp::lib::placeholders::_1;
    using websocketpp::lib::placeholders::_2;

    // Register Event Handlers
    server.set_open_handler(bind(&ChatServer::on_successful_new_connection, this, _1));
    server.set_close_handler(bind(&ChatServer::on_close_connection, this, _1));
    server.set_fail_handler(bind(&ChatServer::on_connection_failed, this, _1));
    server.set_message_handler(bind(&ChatServer::on_message_received, this, _1, _2));

//    Thread command_prompt_thread(&ChatServer::open_command_prompt, this);
//    Thread server_thread(&ChatServer::start, this);
//    server_thread.join();
    start();
//    command_prompt_thread.join();
}

void ChatServer::start() {
    log("Starting server on " + uri);
//    Thread command_prompt_thread(&ChatServer::open_command_prompt, this);
//    Thread server_thread(&Server::run, &server);
    server.listen(port);
    server.start_accept();
    server.run();
//    server_thread.join();
//    command_prompt_thread.join();
}

void ChatServer::stop() {
    log("Server shutting down");
    server.stop_listening();
    for (auto it: connections) {
        ErrorCode error;
        server.close(it, websocketpp::close::status::going_away, "Server shutting down.", error);
        if (error) {
            std::cout << "Error stop(): " << error.message() << std::endl;
        }
    }
    server.stop();
}

void ChatServer::send_message(Connection connection, const std::string &msg) {
    ErrorCode error;
    server.send(connection, msg, websocketpp::frame::opcode::text);
    if (error){
        log("Error sending message: " + error.message());
    }
}

void ChatServer::broadcast_message(const std::string &msg) {
    log("[broadcast_message] " + msg);
    for (auto connection : connections){
        send_message(connection, msg);
    }
}

void ChatServer::on_successful_new_connection(Connection connection) {
    log("New successful connection for client!");
    connections.insert(connection);
    std::string msg = "A user joined the chatroom. Total online users: " + std::to_string(connections.size());
    broadcast_message(msg);
}

void ChatServer::on_connection_failed(Connection connection) {
    log("Connection failed for client!");
}

void ChatServer::on_close_connection(Connection connection) {
    log("Closed connection for client.");
    connections.erase(connection);
}

void ChatServer::on_message_received(Connection connection, Message message) {
    if (message->get_payload() == "User: stopserver"){
        stop();
    } else

    for (auto it: connections) {
        server.send(it, message);
    }
}

void ChatServer::log(const std::string &message) {
    server.get_alog().write(LogLevel::app, message);
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
