//
// Created by Floris Gravendeel on 17/01/2023.
//

#ifndef CARBONCHAT_CHATSERVER_H
#define CARBONCHAT_CHATSERVER_H

#include "chatserver.h"
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/endpoint.hpp>
#include <boost/thread/mutex.hpp>

typedef websocketpp::server<websocketpp::config::asio> Server;
typedef websocketpp::connection_hdl Connection;
typedef Server::message_ptr Message;

/**
 * All documentation has been copied from: https://docs.websocketpp.org/reference_8handlers.html
 */
class ChatServer {
public:
    ChatServer(int port, boost::mutex& mutex);
    void start();
    void stop();
private:
    int get_total_connections() const;

    /// Sends a message to a single connection.
    void send_message(const std::string& msg);

    /// Sends a message to all clients in the network
    void broadcast_message(const std::string& msg);

    /// Either open or fail will be called for each connection. Never both. All connections that
    /// begin with an open handler call will also have a matching close handler call when the connection ends.
    void on_successful_new_connection(Connection connection);

    /// Either open or fail will be called for each connection. Never both.
    /// Connections that fail will never have a close handler called.
    void on_connection_failed(Connection connection);

    /// Close will be called exactly once for every connection that open was called for.
    /// Close is not called for failed connections.
    void on_close_connection(Connection connection);

    /// These handers are called in response to incoming messages or message like events.
    /// They only will be called while the connection is in the open state.
    /// Applies to all non-control messages, including both text and binary opcodes.
    /// The message_ptr type and its API depends on your endpoint type and its config.
    void on_message(const Connection& connection, Message message);

    Server server;
    int port;
    boost::mutex* mutex;
};

#endif //CARBONCHAT_CHATSERVER_H
