//
// Created by Floris Gravendeel on 24/01/2023.
//

#ifndef CARBONCHAT_SERVERCOMMAND_H
#define CARBONCHAT_SERVERCOMMAND_H

#define STOP_COMMAND "/stop" // Usage: /stop Serverside: Disconnect all clients and stops the server immediately.
                             //              Clientside: Leave chatserver (disconnect) and exit process.
#define SHOW_JOIN_MSG_COMMAND "/show-join-msg" // Usage: /show-join-msg <user>
// Format used by the server.
#define JOIN_MSG_FORMAT "%user joined the chat. Total online users: %count"
// Substring used for clients to recognize the join message.
#define JOIN_MSG "joined the chat. Total online users:"

#define SHOW_LEAVE_MSG_COMMAND "/show-leave-msg" // Usage: /show-join-msg <user>
// Format used by the server.
#define LEAVE_MSG_FORMAT "%user left the chat. Total online users: %count"
// Substring used for clients to recognize the leave message.
#define LEAVE_MSG "left the chat. Total online users:"

#define SERVER_GOING_OFFLINE_MSG "Chatserver is going offline." // Announcement for when the server is shutting down.
#define UNIT_TEST_COMMAND "/unit-test"

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>


typedef websocketpp::client<websocketpp::config::asio_client> Client;
typedef websocketpp::connection_hdl Connection;

class ServerCommand {
public:
    ServerCommand(int port, const std::string& command, const std::string& permissions_key);

    /// Executes the command on the server with the given permissions key.
    void execute();

    /// Either open or fail will be called for each connection. Never both. All connections that
    /// begin with an open handler call will also have a matching close handler call when the connection ends.
    void on_successful_new_connection(const Connection& connection);

    void disconnect(const Connection &connection);

private:

    Client client;
    std::string command;
    std::string permissions_key;
    std::string server_uri;
};


#endif //CARBONCHAT_SERVERCOMMAND_H
