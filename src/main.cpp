//
// Created by Floris Gravendeel on 18/01/2023.
//
#include "chatserver.cpp"
#include "chatclient.cpp"
#include "algorithm"

using namespace std;
#define PORT_NUMBER 9002

string prompt(const string& question, vector<string> answers, bool check_answers) {
    cout << question << " [" << Color::FG_LIGHT_GRAY << answers[0] << Color::FG_DEFAULT << "]: ";
    string input;
    getline(cin, input);
    if (input.empty()) {
        return answers[0];
    } else if (!check_answers) {
        return input;
    } else if (find(answers.begin(), answers.end(), input) != answers.end()) {
        return input;
    } else {
        cout << "Please enter a valid answer. ";
        for (const string& x : answers){
            cout << "[" << Color::FG_LIGHT_GRAY << x << Color::FG_DEFAULT << "] ";
        }
        cout << endl;
        return prompt(question, answers, check_answers);
    }
}
void server_command_line_interface() {
    std::string input;
    while (true) {
        //Read user input from stdin
        std::getline(std::cin, input);
        if (input == STOP_COMMAND) {
            ServerCommand stopCommand(PORT_NUMBER, STOP_COMMAND, "AKEY");
            stopCommand.execute();
            break;
        }
    }
    std::cout << "Closing Server CLI." << std::endl;
}

int main(int argc, char** argv) {

    cout << Color::FG_YELLOW << "Starting up Carbon-Chat" << Color::FG_DEFAULT << endl;
    cout << "Leave blank and press enter to use the default"
    << " [" << Color::FG_LIGHT_GRAY << "value" << Color::FG_DEFAULT << "]." << endl;
    bool selected_server = prompt("Do you want to host or join a chatserver?",
                         {"host", "join"}, true) == "host";

    string username;
    string ip;
    int port;
    if (selected_server) {
        port = std::stoi(prompt("Which port should the server be hosted on?",
                                {to_string(PORT_NUMBER)}, false));
        cout << Color::FG_YELLOW << "Launching server.\nTo stop the server. Use command: " << Color::BOLD << "/stop"
             << Color::RESET_ALL_ATTRIBUTES << endl;

        // Create an instance of Chatserver (the server starts automatically)
        ChatServer server(port);
        server.start();
    } else { // Else start the client
        username = prompt("Please enter a username", {"User"}, false);
        cout << "Hi " << Color::FG_CYAN << username << Color::FG_DEFAULT << ". ";
        ip = prompt("What is the IP address of the server?", {"localhost"}, false);
        port = std::stoi(prompt("And which port?", {to_string(PORT_NUMBER)}, false));

        cout << Color::FG_YELLOW << "Launching client.\nTo stop the client. Use command: " << Color::BOLD << "/stop"
             << Color::RESET_ALL_ATTRIBUTES << endl;
//        thread cli(&server_command_line_interface);
//        cli.join();
        ChatClient client(ip, port, username, false);
        client.start();
    }


    return 0;
}

