//
// Created by Floris Gravendeel on 20/01/2023.
//
#include "chatclient.h"
using namespace std;

ChatClient::ChatClient(const std::string host, int port, bool debug = true) {
    server_uri = "ws://localhost:9002";
    if (debug) {
        client.set_access_channels(websocketpp::log::alevel::all);
        client.clear_access_channels(websocketpp::log::alevel::frame_payload);
    } else {
        client.set_access_channels(websocketpp::log::alevel::none);
    }
    client.init_asio();
    // Register Event Handler
    client.set_message_handler(bind(&ChatClient::on_message, this, websocketpp::lib::placeholders::_1,
                                    websocketpp::lib::placeholders::_2));
}

void ChatClient::start() {
    log("Connecting to server at: " + server_uri);
    try {
        websocketpp::lib::error_code ec;
        Client::connection_ptr connection = client.get_connection(server_uri, ec);
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            return;
        }

        // Note that connect here only requests a connection. No network messages are
        // exchanged until the event loop starts running in the next line.
        client.connect(connection);

        // Start the ASIO io_service run loop
        // this will cause a single connection to be made to the server. c.run()
        // will exit when this connection is closed.
        client.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }
    stop();
}

void ChatClient::stop() {
    websocketpp::lib::error_code ec;
    Client::connection_ptr connection = client.get_connection(server_uri, ec);
    if (ec){
        log(ec.message());
    }

    connection->close(websocketpp::close::status::going_away, "Exiting program");
}

void ChatClient::send_message(const std::string &msg) {

}

void ChatClient::on_message(const Connection &connection, const Message &message) {
    log(message->get_payload());
}

void ChatClient::log(const std::string &message) {
    cout << message << endl;
//    client.get_alog().write(websocketpp::log::alevel::app, message);
}
