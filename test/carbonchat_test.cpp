//
// Created by Floris Gravendeel on 18/01/2023.
//
#define BOOST_TEST_MODULE TestChatServer
#include <boost/test/unit_test.hpp>
#include "../src/chatserver.cpp"
#include "../src/servercommand.cpp"


/// This unit tests starts up 1 server and 2 clients. Once the server goes online, the unit-test command is run.
/// Both clients will sent 50 messages. The server has to receive 100 messages in order for the unit test to pass.
BOOST_AUTO_TEST_CASE(ChatServerCase)
{
    int port = 9002;
    ChatServer chatServer(port);

    std::thread serverThread([&chatServer]() {
        chatServer.start();
    });
    std::this_thread::sleep_for(std::chrono::seconds (5)); // Wait 5 seconds for the server to start up.
    ServerCommand stopCommand(port, STOP_COMMAND, chatServer.get_permissions_key());
    stopCommand.execute();
    serverThread.join();

    // To simplify this example test, let's suppose we'll test 'float'.
    // Some test are stupid, but all should pass.
    float x = 9.5f;

    BOOST_CHECK(x != 0.0f);
    BOOST_CHECK_EQUAL((int) x, 9);
    BOOST_CHECK_CLOSE(x, 9.5f, 0.0001f); // Checks differ no more then 0.0001%
}