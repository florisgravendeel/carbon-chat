//
// Created by Floris Gravendeel on 18/01/2023.
//
#include "chatserver.cpp"
#include "chatclient.cpp"
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
using namespace std;

#define PORT_NUMBER 9002


int main(int argc, char** argv) {

    if (argc != 2) {
        cout << "Please start with argument. -server or -client" << endl;
        return 0;
    }
    string option(argv[1]);

    if (option == "-server") {
        cout << "To stop the server. Use command: stop\n";
//        Chatserver server(PORT_NUMBER, s_mutex);

    } else if (option == "-client"){
        cout << "Launching client.\nTo stop the client. Use command: stop" << endl;
        ChatClient client("localhost", 9002, false);
        client.start();

    } else {
        cout << "Not a valid start argument. Choose between -server or -client";
    }

    return 0;
}
