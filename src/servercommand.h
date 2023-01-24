//
// Created by Floris Gravendeel on 24/01/2023.
//

#ifndef CARBONCHAT_SERVERCOMMAND_H
#define CARBONCHAT_SERVERCOMMAND_H

#define STOP_COMMAND "/stop"
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
    bool command_executed;
};


#endif //CARBONCHAT_SERVERCOMMAND_H
