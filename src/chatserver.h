//
// Created by Floris Gravendeel on 17/01/2023.
//

#ifndef CARBONCHAT_CHATSERVER_H
#define CARBONCHAT_CHATSERVER_H

class ChatServer {
public:
    ChatServer(int port);
private:
    int port;
    void run();
};

#endif //CARBONCHAT_CHATSERVER_H
