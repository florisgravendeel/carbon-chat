//
// Created by Floris Gravendeel on 18/01/2023.
//
#include "chatserver.cpp"
#include "chatclient.cpp"
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
using namespace std;

#define PORT_NUMBER 9002

ChatServer* server;
boost::mutex s_mutex;

void start_thread(){
    // Create instance of Chatserver.
    server = new ChatServer(PORT_NUMBER, s_mutex);
    server->start();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Please start with argument. -server or -client";
        return 0;
    }
    string option(argv[1]);

    if (option == "-server") {
        cout << "To stop the server. Use command: stop\n";

        boost::thread server_thread(start_thread);
        string input;
        while (true) {
            getline(cin, input);
            if (input == "stop") {
                s_mutex.lock();
                server->stop();
                s_mutex.unlock();
                server_thread.join();
                break;
            }
            cout << "Invalid command." << endl;
        }
    } else if (option == "-client"){
        ChatClient client("localhost", PORT_NUMBER);
        client.start();
    } else {
        cout << "Not a valid start argument. Choose between -server or -client";
    }

    return 0;
}
