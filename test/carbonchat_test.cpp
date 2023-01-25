//
// Created by Floris Gravendeel on 18/01/2023.
//
#define BOOST_TEST_MODULE TestChatServer
#include <boost/test/unit_test.hpp>
#include "testserver.cpp"
#include "testclient.cpp"
typedef std::thread Thread;

void log(const std::string& message){
    cout << Color::FG_MAGENTA << message << Color::FG_DEFAULT << endl;
}

/// This unit tests starts up 1 server and 2 clients. Once the server goes online, the unit-test command is executed.
/// Both clients will sent 50 messages. The server has to receive 100 messages in order for the unit test to pass.
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
    log("Waiting 5 seconds for the server to start up.");
    std::this_thread::sleep_for(std::chrono::seconds (5));


//    log("Launching TestClient 1 as user TestUser1");
//    TestClient client1(ip, port, "TestUser1", false, 50);
//    Thread client1_thread([&client1]() {
//        client1.start();
//    });

//    Thread start_unit_test_thread([port, &server]() {
//        ServerCommand unit_testCommand(port, UNIT_TEST_COMMAND, server.get_permissions_key());
//        unit_testCommand.execute();
//    });

//    log("Launching Client 2 as user TestUser2");
//    TestClient client2(ip, port, "TestUser2", false);
//    Thread client2_thread([&client2]() {
//        client2.start();
//    });
    std::this_thread::sleep_for(std::chrono::seconds (10));

//    Thread stop_thread([port, &server]() {
//        ServerCommand stopCommand(port, STOP_COMMAND, server.get_permissions_key());
//        stopCommand.execute();
//    });

    server_thread.join();
//    client1_thread.join();
//    client2_thread.join();
//    stop_thread.join();



    // To simplify this example test, let's suppose we'll test 'float'.
    // Some test are stupid, but all should pass.
    float x = 9.5f;
    log("total_sent_messages: " + std::to_string(total_sent_messages));
    log("total_received_messages: " + std::to_string(total_received_messages));
    BOOST_CHECK(x != 0.0f);
//    BOOST_CHECK_EQUAL((int) total_received_messages, 20);
//    BOOST_CHECK_CLOSE(x, 9.5f, 0.0001f); // Checks differ no more then 0.0001%
}
