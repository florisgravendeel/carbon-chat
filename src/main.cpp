//
// Created by Floris Gravendeel on 18/01/2023.
//
#include "chatserver.cpp"
#include "chatclient.cpp"

// Copied from (https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal)
#include <ostream>
namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49, //

        FG_BLACK    = 30,
        FG_YELLOW   = 33,
        FG_MAGENTA  = 35,
        FG_CYAN     = 36,
        FG_LIGHT_GRAY   = 37,
        FG_DARK_GRAY    = 90,
        FG_LIGHT_RED    = 91,
        FG_LIGHT_GREEN  = 92,
        FG_LIGHT_YELLOW = 93,
        FG_LIGHT_BLUE   = 94,
        FG_LIGHT_MAGENTA= 95,
        FG_LIGHT_CYAN   = 96,
        FG_WHITE        = 97,
    };
    std::ostream& operator<<(std::ostream& os, Code code) {
        return os << "\033[" << static_cast<int>(code) << "m";
    }
}
using namespace std;
#define PORT_NUMBER 9002


int main(int argc, char** argv) {
    cout << Color::FG_YELLOW << "Starting up Carbon-Chat client" << Color::FG_DEFAULT << endl;
    cout << "Please enter a username [" << Color::FG_LIGHT_GRAY << "User" << Color::FG_DEFAULT << "]:" << endl;
    cout << "Hi User. Which server should I connect to? [" << Color::FG_LIGHT_GRAY << "localhost" << Color::FG_DEFAULT << "]:" << endl;

//    Connecting to localhost..
//            Chatserver offline. Stopping client

    return 0;
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
