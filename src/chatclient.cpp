//
// Created by Floris Gravendeel on 20/01/2023.
//
#include "chatclient.h"
#include "servercommand.h"
#include "colored_terminal.cpp"
using namespace std;

ChatClient::ChatClient(const string &host, int port, const string &username, bool debug = false) {
    connection_open = false;
    connection_closed = false;
    chat_prompt_disabled = true;
    chat_prompt_active = false;
    ChatClient::username = username;
    server_uri = "ws://" + host + ":" + to_string(port);

    if (debug) { // Only log interesting things
        client.clear_access_channels(LogLevel::all);
        client.set_access_channels(LogLevel::connect);
        client.set_access_channels(LogLevel::disconnect);
        client.set_access_channels(LogLevel::app);
    } else {
        client.clear_access_channels(LogLevel::none);
        client.set_access_channels(LogLevel::none);
    }

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

    ErrorCode error;
    Client::connection_ptr con = client.get_connection(server_uri, error);
    if (error) {
        log("Connection error: " + error.message(), LogType::Error);
        return;
    }

    // Documentation: The connection handle is a token that can be shared outside the WebSocket++ core for
    // the purposes of identifying a connection and sending it messages.
    connection = con->get_handle();

    // Queue the connection. No network connections will be made until the io_service is running.
    client.connect(con);
    log("Connecting to localhost..", LogType::Info);

    // Create a thread to run the asio io_service.
    Thread asio_thread(&Client::run, &client);
    Thread* chat_prompt_thread;

    if (chat_prompt_disabled){
        asio_thread.join();
    } else {
        // Create a thread to open the chat prompt.
        chat_prompt_thread = new Thread(&ChatClient::open_chat_prompt, this);
        chat_prompt_active = true;
        asio_thread.join();
        chat_prompt_thread->join();
    }
}

void ChatClient::stop() {
    log("Leaving chatserver!", LogType::Info);
    client.close(connection, websocketpp::close::status::going_away, "Leaving chat");
}

void ChatClient::on_successful_new_connection(const Connection &connection) {
    log("Chatserver online.", LogType::Success);
    ScopedLock guard(mutex);
    connection_open = true;
    std::string command = SHOW_JOIN_MSG_COMMAND; // Sending /show-join-msg <user>
    client.send(ChatClient::connection, command + " " + username, websocketpp::frame::opcode::text);
}

void ChatClient::on_connection_failed(const Connection &connection) {
    log("Failed to connect to chatserver!", LogType::Error);
    ScopedLock guard(mutex);
    connection_closed = true;
}

void ChatClient::on_close_connection(const Connection &connection) {
    ScopedLock guard(mutex);
    connection_closed = true;
    log("Connection closed successfully.", LogType::Success);

    if (chat_prompt_active){
        // Due to the nature of threads, the user has to press enter to exit the chat prompt if the server goes offline.
        // This message is not needed when client uses the /stop command (because it exits the chat thread).
        log("Press enter to exit chat prompt.", LogType::Info);
    }
}

void ChatClient::on_message_received(const Connection &connection, const Message &message) {
    std::string msg = message->get_payload();
    if (boost::algorithm::contains(msg, JOIN_MSG)) {
        log(msg, LogType::UserJoinedServer);
    } else if (msg == SERVER_GOING_OFFLINE_MSG){
        log(msg, LogType::ServerAnnouncement);
    } else {
        log(msg, LogType::Chat);
    }
}

void ChatClient::log(const string &message, ChatClient::LogType logType) {
    if (chat_prompt_disabled){
        return;
    }
        switch (logType) {
            case Info:
                cout << Color::FG_YELLOW << message << Color::FG_DEFAULT << endl;
                break;
            case UserJoinedServer:
            {
                auto player_count = message.back();
                std::string join_msg = message;
                join_msg.pop_back();
                cout << Color::FG_LIGHT_YELLOW << join_msg << Color::FG_MAGENTA << player_count
                << Color::FG_DEFAULT << endl;
                break;
            }
            case Chat:
            {
                char *msg = new char[message.length() + 1];
                strcpy(msg, message.c_str());

                char *ptr;
                ptr = strtok(msg, ":");
                short i = 0;
                while (ptr != nullptr) {
                    if (i == 0) {
                        cout << Color::FG_CYAN << ptr << ":" << Color::FG_LIGHT_CYAN;
                    } else {
                        cout << ptr;
                    }
                    ptr = strtok(nullptr, ":");
                    i++;
                }
                cout << Color::FG_DEFAULT << endl;
                break;
            }
            case ServerAnnouncement:
            {
                cout << Color::FG_LIGHT_YELLOW << message << Color::FG_DEFAULT << endl;
                break;
            }
            case Error:
                cout << Color::FG_RED << message << Color::FG_DEFAULT << endl;
                break;
            case Success:
                cout << Color::FG_GREEN << message << Color::FG_DEFAULT << endl;
                break;
        }
}

void ChatClient::open_chat_prompt(){
    string input;
    while (true)
    {
        bool wait = false;
        {
            ScopedLock guard(mutex);
            // If the connection hasn't been opened yet wait a bit and retry.
            if (!connection_open) {
                wait = true;
            }
        }
        if (wait) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            continue;
        }
        //Read user input from stdin
        std::getline(std::cin, input);
        // Stop if the connection has been closed.
        if (connection_closed) {
            break;
        }
        if (input == STOP_COMMAND){
            chat_prompt_active = false;
            stop();
            break;
        }
        ErrorCode error;

        client.send(connection, username + ": " + input, websocketpp::frame::opcode::text, error);
        if (error) {
            log("Error sending message: " + error.message(), LogType::Error);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
