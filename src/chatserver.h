//
// Created by Floris Gravendeel on 17/01/2023.
//

#ifndef CARBONCHAT_CHATSERVER_H
#define CARBONCHAT_CHATSERVER_H

#include <set>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/common/thread.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/algorithm/string.hpp>

typedef websocketpp::server<websocketpp::config::asio> Server;
typedef websocketpp::connection_hdl Connection;
typedef Server::message_ptr Message;
typedef std::set<Connection,std::owner_less<Connection>> ConnectionList;

typedef websocketpp::log::alevel LogLevel;
typedef websocketpp::lib::thread Thread;
typedef websocketpp::lib::error_code ErrorCode;

/**
 * Almost all documentation has been copied from: https://docs.websocketpp.org/reference_8handlers.html
 */
class ChatServer {
public:
    ChatServer(int port);

    std::string get_permissions_key();

     /// On data message received from WebSocket. This is method is called by a handler.
    virtual void on_message_received(const Connection& connection, const Message& message);

    /// WebSocket++ has the capability of logging events during the lifetime of the connections that it processes.
    /// Each endpoint has two independent logging interfaces that are used by all connections created by that endpoint.
    /// This method uses the an access interface for application specific logs.
    void log(const std::string& message);

    /// Starts the server inside a thread.
    void start();

    /// This method should only be used by the server. See /stop command.
    void stop();

    /// Sends a message to all clients in the network.
    virtual void broadcast_message(const std::string& msg);

private:

    /// Sends a message to a single connection.
    void send_message(Connection connection, const std::string& msg);

    /// Either open or fail will be called for each connection. Never both. All connections that
    /// begin with an open handler call will also have a matching close handler call when the connection ends.
    void on_successful_new_connection(const Connection& connection);

    /// Either open or fail will be called for each connection. Never both.
    /// Connections that fail will never have a close handler called.
    void on_connection_failed(Connection connection);

    /// Close will be called exactly once for every connection that open was called for.
    /// Close is not called for failed connections.
    void on_close_connection(const Connection& connection);

    /// This method generates a permissions key using the Boost UUID Library.
    /// This key required to run all defined commands in servercommand.h header file.
    static std::string generate_permissions_key();

    Server server;
    std::string uri;
    int port;
    ConnectionList connections;
    std::string permissions_key; // This key allows users to enter privileged commands.
    bool initiating_shutdown;

};

#endif //CARBONCHAT_CHATSERVER_H
