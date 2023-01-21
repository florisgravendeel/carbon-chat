//
// Created by Floris Gravendeel on 18/01/2023.
//
#include "chatserver.cpp"
#include "chatclient.cpp"


#include "colored_terminal.cpp"
#include "algorithm"
using namespace std;
#define PORT_NUMBER 9002

string prompt(string question, vector<string> answers, bool check_answers) {
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
void info(string msg){
    cout << Color::FG_YELLOW << msg << Color::FG_DEFAULT << endl;
}

int main(int argc, char** argv) {
    cout << Color::FG_YELLOW << "Starting up Carbon-Chat" << Color::FG_DEFAULT << endl;
    bool server = prompt("Do you want to host or join a chatserver?",
                         {"host", "join"}, true) == "host";
    string username;
    if (!server){
        username = prompt("Please enter a username", {"User"}, false);
        cout << "Hi " << Color::FG_CYAN << username << Color::FG_DEFAULT << ". ";
    }
    string ip = prompt("What is the IP address of the server?", {"localhost"}, false);

    int port = std::stoi(prompt("And which port?", {to_string(PORT_NUMBER)}, false));

    if (server) {
        cout << Color::FG_YELLOW << "Launching server.\nTo stop the server. Use command: " << Color::BOLD << "/stop"
        << Color::FG_DEFAULT << endl;
    } else {
        cout << Color::FG_YELLOW <<"Launching client.\nTo stop the client. Use command: " << Color::BOLD << "/stop"
        << Color::FG_DEFAULT << endl;
        ChatClient client(ip, port, false);
        client.start();
    }

    return 0;
}
