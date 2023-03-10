//
// Created by Floris Gravendeel on 25/01/2023.
//
#include "../src/chatserver.cpp"
#include <boost/algorithm/string/predicate.hpp>

/// TestServer inherits ChatServer. TestServer behaves the same as ChatClient, but also keeps count of
/// sent/received messages.
class TestServer : public ChatServer
{
    int &total_received_messages;
    int &total_sent_messages;
public:
    TestServer(int port, int &total_received_messages, int &total_sent_messages)
        : ChatServer(port), total_received_messages(total_received_messages), total_sent_messages(total_sent_messages) {
    }

    void on_message_received(const Connection &connection, const Message &message) override {
        total_received_messages++;
        ChatServer::on_message_received(connection, message);
    }

    void broadcast_message(const std::string &msg) override {
        if (msg == SERVER_GOING_OFFLINE_MSG){ // Disable "Chatserver is going offline.", for more accurate test results.
            return;
        }
        total_sent_messages++;
        ChatServer::broadcast_message(msg);
    }
};