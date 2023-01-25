//
// Created by Floris Gravendeel on 18/01/2023.
//
#define BOOST_TEST_MODULE TestChatServer
#include <boost/test/unit_test.hpp>
#include "testserver.cpp"
#include "testclient.cpp"
#include "../src/servercommand.cpp"

typedef std::thread Thread;

void log(const std::string& message){
    cout << Color::FG_MAGENTA << message << Color::FG_DEFAULT << endl;
}

/// This unit test starts up a test server and 2 test clients.
/// Each client will sent 10 messages in total.
/// For the test to pass to following needs to happen:
/// - The server has to receive 20 messages (incoming data).
/// - The server has to broadcast the 20 sent messages (outgoing data).
BOOST_AUTO_TEST_CASE(ChatServerCase)
{
    int port = 9002;
    std::string ip = "localhost";

    log("Starting server.");
    int total_received_messages = 0; // Pass object by reference, start at 0 messages and with 100 messages.
    int total_sent_messages = 0;
    TestServer server(port, total_received_messages, total_sent_messages);
    Thread server_thread([&server]() {
        server.start();
    });
    log("Waiting 3 seconds for the server to start up.");
    std::this_thread::sleep_for(std::chrono::seconds (3));


    log("Launching TestClient 1 as user TestUser1");
    TestClient client1(ip, port, "TestUser1", false, 10);
    Thread client1_thread([&client1]() {
        client1.start();
    });

    log("Launching TestClient 2 as user TestUser2");
    TestClient client2(ip, port, "TestUser2", false, 10);
    Thread client2_thread([&client2]() {
        client2.start();
    });

    std::this_thread::sleep_for(std::chrono::seconds (2)); // Sleep for a bit.
    log("All 20 messages should be sent by now. Stopping server.");

    server.stop();
    server_thread.join();
    client1_thread.join();
    client2_thread.join();

    BOOST_CHECK(total_received_messages == 20);
    BOOST_CHECK(total_sent_messages == 20);
}
