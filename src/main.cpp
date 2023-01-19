//
// Created by Floris Gravendeel on 18/01/2023.
//
#include "chatserver.cpp"
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
using namespace std;

ChatServer* server;
boost::mutex s_mutex;

void start_thread(){
    // Create instance of Chatserver.
    server = new ChatServer(9002, s_mutex);
    server->start();
}

int main() {

    boost::thread server_thread(start_thread);

    string input;
    cout << "To stop the server. Use command: stop\n";
    while (true)
    {
        getline(cin, input);
        if (input == "stop"){
            s_mutex.lock();
            server->stop();
            s_mutex.unlock();
            break;
        }
        cout << "Invalid command." << endl;
    }

    return 0;
}
