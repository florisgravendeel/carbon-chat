//
// Created by Floris Gravendeel on 25/01/2023.
//
#include "../src/chatserver.cpp"
class TestServer : public ChatServer
{
    int &total_received_messages;
    int &total_sent_messages;
public:
    TestServer(int port, int &total_received_messages, int &total_sent_messages)
        : ChatServer(port), total_received_messages(total_received_messages), total_sent_messages(total_sent_messages) {
    }

private:
    void on_message_received(const Connection &connection, const Message &message) override {
        total_received_messages++;
        if (total_received_messages == 10){
            log("Total received messages: " + std::to_string(total_received_messages));
            stop();
            return;
        }
        ChatServer::on_message_received(connection, message);
    }

public:
    void broadcast_message(const std::string &msg) override {
        total_sent_messages++;
        ChatServer::broadcast_message(msg);
    }
};