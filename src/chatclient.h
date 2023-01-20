//
// Created by Floris Gravendeel on 20/01/2023.
//

#ifndef CARBONCHAT_CHATCLIENT_H
#define CARBONCHAT_CHATCLIENT_H

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <iostream>

typedef websocketpp::client<websocketpp::config::asio_client> Client;

typedef websocketpp::connection_hdl Connection;
typedef Client::message_ptr Message;

class ChatClient {
public:
    ChatClient(const std::string host, int port);
    void start();
    void stop();
private:

    /// Sends a message to all other users in the chat.
    void send_message(const std::string& msg);

    /// On data message received from WebSocket. This is method is called by a handler.
    void on_message(const Connection& connection, const Message& message);

    /// WebSocket++ has the capability of logging events during the lifetime of the connections that it processes.
    /// Each endpoint has two independent logging interfaces that are used by all connections created by that endpoint.
    /// The method uses the an access interface for application specific logs.
    void log(const std::string& message);

    Client client;
};


#endif //CARBONCHAT_CHATCLIENT_H
