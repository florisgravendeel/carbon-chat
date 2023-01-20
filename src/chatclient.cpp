//
// Created by Floris Gravendeel on 20/01/2023.
//
#include "chatclient.h"
using namespace std;

ChatClient::ChatClient(const std::string host, int port) {
    try {
        std::string uri = "ws://localhost:9002";
        log("Connecting to server at: " + uri);

        // Set logging to be pretty verbose (everything except message payloads)
        client.set_access_channels(websocketpp::log::alevel::all);
        client.clear_access_channels(websocketpp::log::alevel::frame_payload);
        client.set_error_channels(websocketpp::log::elevel::all);

        // Initialize ASIO
        client.init_asio();

        // Register our message handler
        client.set_message_handler(bind(&ChatClient::on_message, this, std::placeholders::_1, std::placeholders::_2));

        websocketpp::lib::error_code ec;
        Client::connection_ptr con = client.get_connection(uri, ec);
        if (ec) {
            std::cout << "Could not create connection because: " << ec.message() << std::endl;
            return;
        }

        // Note that connect here only requests a connection. No network messages are
        // exchanged until the event loop starts running in the next line.
        client.connect(con);

        // Start the ASIO io_service run loop
        // this will cause a single connection to be made to the server. c.run()
        // will exit when this connection is closed.
        client.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }
}

void ChatClient::start() {

}

void ChatClient::stop() {

}

void ChatClient::send_message(const std::string &msg) {

}

void ChatClient::on_message(const Connection &connection, const Message &message) {
    log(message->get_payload());
}

void ChatClient::log(const std::string &message) {
    client.get_alog().write(websocketpp::log::alevel::app, message);
}
