//
// Created by Floris Gravendeel on 20/01/2023.
//
#include "chatclient.h"

ChatClient::ChatClient(const string &host, int port, bool debug) {
    connection_open = false;
    connection_closed = false;
    server_uri = "ws://" + host + ":" + to_string(port);

    // Only log interesting things
    client.clear_access_channels(websocketpp::log::alevel::all);
    client.set_access_channels(websocketpp::log::alevel::connect);
    client.set_access_channels(websocketpp::log::alevel::disconnect);
    client.set_access_channels(websocketpp::log::alevel::app);

    // Documentation: Initialize asio transport with internal io_service
    client.init_asio();

    using websocketpp::lib::placeholders::_1;
    using websocketpp::lib::placeholders::_2;
    using websocketpp::lib::bind;

    client.set_message_handler(bind(&ChatClient::on_message_received, this, _1, _2));
    client.set_open_handler(bind(&ChatClient::on_successful_new_connection, this, _1));
    client.set_close_handler(bind(&ChatClient::on_close_connection, this, _1));
    client.set_fail_handler(bind(&ChatClient::on_connection_failed, this, _1));
}

void ChatClient::start() {

    websocketpp::lib::error_code ec;
    Client::connection_ptr con = client.get_connection(server_uri, ec);
    if (ec) {
        client.get_alog().write(websocketpp::log::alevel::app,"Connection Error: " + ec.message());
        return;
    }

    // Documentation: The connection handle is a token that can be shared outside the WebSocket++ core for
    // the purposes of identifying a connection and sending it messages.
    connection = con->get_handle();

    // Queue the connection. No network connections will be made until the io_service is running.
    client.connect(con);

    // Create a thread to run the asio io_service.
    websocketpp::lib::thread asio_thread(&Client::run, &client);

    // Create a thread to open the chat prompt.
    websocketpp::lib::thread chat_prompt(&ChatClient::chat_prompt, this);
    asio_thread.join();
    chat_prompt.join();
}

void ChatClient::stop() {
    client.get_alog().write(websocketpp::log::alevel::app, "Exiting client! ");
    client.close(connection, websocketpp::close::status::going_away, "Leaving chat");
}

void ChatClient::send_message(const string &msg) {

}

void ChatClient::on_successful_new_connection(const Connection &connection) {
    client.get_alog().write(websocketpp::log::alevel::app,"Chatserver online!");

    ScopedLock guard(mutex);
    connection_open = true;
}

void ChatClient::on_connection_failed(const Connection &connection) {
    client.get_alog().write(websocketpp::log::alevel::app,"Failed to connect to chatserver!");

    ScopedLock guard(mutex);
    connection_closed = true;
}

void ChatClient::on_close_connection(const Connection &connection) {
    client.get_alog().write(websocketpp::log::alevel::app,"Leaving chatserver!");

    ScopedLock guard(mutex);
    connection_closed = true;
}

void ChatClient::on_message_received(const Connection &connection, const Message &message) {
    std::cout << message->get_payload() << std::endl;
}

void sleep() {
// Define a semi-cross platform helper method that waits/sleeps for a bit.
#ifdef WIN32
    Sleep(1000);
#else
    sleep(3);
#endif
}

void ChatClient::chat_prompt(){
    string input;
    while (true)
    {
        bool wait = false;

        {
            ScopedLock guard(mutex);
            // If the connection has been closed, stop generating telemetry
            if (connection_closed) {break;}

            // If the connection hasn't been opened yet wait a bit and retry
            if (!connection_open) {
                wait = true;
            }
        }

        if (wait) {
            sleep();
            continue;
        }

        //Read user input from stdin
        std::getline(std::cin, input);
        if (input == "/stop"){
            client.get_alog().write(websocketpp::log::alevel::app, "Stopping chatprompt!");
            stop();
            break;
        }
        websocketpp::lib::error_code ec;
        client.send(connection, input, websocketpp::frame::opcode::text, ec);
        if (ec) {
            client.get_alog().write(websocketpp::log::alevel::app,"Send Error: "+ec.message());
            break;
        }
        sleep();
    }
}

void ChatClient::log(const string &message) {

}
