//
// Created by Floris Gravendeel on 20/01/2023.
//

#ifndef CARBONCHAT_CHATCLIENT_H
#define CARBONCHAT_CHATCLIENT_H

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>

#include <iostream>

typedef websocketpp::client<websocketpp::config::asio_client> Client;
typedef websocketpp::lib::lock_guard<websocketpp::lib::mutex> ScopedLock;
typedef websocketpp::config::asio_client::message_type::ptr Message;
typedef websocketpp::connection_hdl Connection;
typedef websocketpp::lib::thread Thread;
typedef websocketpp::log::alevel LogLevel;
typedef websocketpp::lib::error_code ErrorCode;

class ChatClient {
public:
    ChatClient(const std::string& host, int port, const std::string& username, bool debug);

    virtual void start();

    virtual void stop();
private:

    /// Either open or fail will be called for each connection. Never both. All connections that
    /// begin with an open handler call will also have a matching close handler call when the connection ends.
    void on_successful_new_connection(const Connection& connection);

    /// Either open or fail will be called for each connection. Never both.
    /// Connections that fail will never have a close handler called.
    void on_connection_failed(const Connection& connection);

    /// Close will be called exactly once for every connection that open was called for.
    /// Close is not called for failed connections.
    void on_close_connection(const Connection& connection);

    /// On data message received from WebSocket. This is method is called by a handler.
    void on_message_received(const Connection& connection, const Message& message);

    /// The chat prompt allows the user to type in and send messages to all the other users in the chat.
    void open_chat_prompt();

    enum LogType { Info, UserJoinedServer, Chat, Error, Success};
    /// WebSocket++ has the capability of logging events during the lifetime of the connections that it processes.
    /// Each endpoint has two independent logging interfaces that are used by all connections created by that endpoint.
    /// The method uses the an access interface for application specific logs.
    static void log(const std::string& message, LogType logType);

    Client client;
    Connection connection;
    websocketpp::lib::mutex mutex;
    bool connection_open;
    bool connection_closed;
    std::string server_uri;
    std::string username;
};


#endif //CARBONCHAT_CHATCLIENT_H
