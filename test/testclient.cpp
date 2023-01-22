//
// Created by Floris Gravendeel on 22/01/2023.
//
#include "../src/chatclient.h"
#include "../src/chatclient.cpp"

class TestClient : private ChatClient
{
    TestClient(const string &host, int port, const string &username, bool debug) :
    ChatClient(host, port, username, debug) {

    }

    void start() override {
        ChatClient::start();
    }

    void stop() override {
        ChatClient::stop();
    }
};