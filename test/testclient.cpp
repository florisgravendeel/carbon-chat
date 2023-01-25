//
// Created by Floris Gravendeel on 22/01/2023.
//
#include "../src/chatclient.cpp"

/// TestClient inherits ChatClient. TestClients behaves the same as ChatClient, except that it has no chat prompt or
/// chat output.
/// After starting up the TestClient will send total of 50 messages to the server when the unit test command is received.
class TestClient : public ChatClient
{
    int total_messages_to_send;
    int &total_received_messages;

public:
    TestClient(const string &host, int port, const string &username, bool debug, int total_messages,
               int &total_received_messages) :
    ChatClient(host, port, username, debug), total_received_messages(total_received_messages) {
        TestClient::total_messages_to_send = total_messages;
    }

    void start() override {
        chat_prompt_disabled = true;
        ChatClient::start();
    }

    void on_message_received(const Connection &connection, const Message &message) override {
        total_received_messages++;
        ChatClient::on_message_received(connection, message);
    }

    /// This methods sends a total of 50 messages to the server once connected.
    /// Between each message this thread sleeps approximately 100 milliseconds.
    void on_successful_new_connection(const Connection &connection) override {
        for (int i = 1; i < (total_messages_to_send + 1); i++) {
            client.send(connection, "Test " + to_string(i), websocketpp::frame::opcode::text);
            this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
};