#include "chatserver.h"
#include <iostream>

ChatServer::ChatServer(int port) {
    ChatServer::port = port;
    run();
};

void ChatServer::run() {
    std::cout << &"Starting server on port " [ port];
}