//
// Created by Floris Gravendeel on 18/01/2023.
//
#include "chatserver.cpp"
#include "chatclient.cpp"
#include "servercommand.cpp"
#include "algorithm"
#include <thread>
#include <iostream>


using namespace std;
#define PORT_NUMBER 9002

/// Simple prompt mechanism. This allows us to ask questions to the user running program.
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

/// Opens the command prompt for the server. This allows the user to send commands to the server.
void server_command_line_interface(const std::string& permissions_key) {
    std::string input;
    while (true) {
        //Read user input from stdin
        std::getline(std::cin, input);
        if (input == STOP_COMMAND) {
            ServerCommand stopCommand(PORT_NUMBER, STOP_COMMAND, permissions_key);
            stopCommand.execute();
            break;
        }
    }
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
        // The Command Line Interface allows us to enter commands for the server.
        // We need a permissions key so others cannot send malicious commands.
        thread server_cli(&server_command_line_interface, server.get_permissions_key());
        server.start();
        server_cli.join();
    } else { // Else start the client
        username = prompt("Please enter a username", {"User"}, false);
        cout << "Hi " << Color::FG_CYAN << username << Color::FG_DEFAULT << ". ";
        ip = prompt("What is the IP address of the server?", {"localhost"}, false);
        port = std::stoi(prompt("And which port?", {to_string(PORT_NUMBER)}, false));

        cout << Color::FG_YELLOW << "Launching client.\nTo stop the client. Use command: " << Color::BOLD << "/stop"
             << Color::RESET_ALL_ATTRIBUTES << endl;

        ChatClient client(ip, port, username, false);
        client.start();
    }
    return 0;
}