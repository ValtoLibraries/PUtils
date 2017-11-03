#include <thread>
#include "gtest/gtest.h"
#include "connection/TCPConnection.hpp"
#include "connection/TCPListener.hpp"

TEST(ConnectionTest, ClientServer)
{
    std::atomic<bool> running = true;

    bool serverReceived = false;

    putils::TCPListener server(4242);
    server += [&serverReceived, &server]
    {
        auto &c = server.getLastClient();
        c.newMessage += [&c, &serverReceived]
        {
            serverReceived = true;
            c.send("server to client\n");
        };
    };

    std::thread t([&server, &running]
                  {
                      while (running)
                          server.select({ 0, 0 });
                  }
    );

    putils::TCPConnection client("127.0.0.1", 4242);
    client.send("client to server\n");
    EXPECT_EQ(client.receive(), "server to client\n");
    EXPECT_TRUE(serverReceived);

    running = false;
    t.join();
}

TEST(ConnectionTest, ClientDisconnected)
{
    std::atomic<bool> running = true;
    bool disconnected = false;

    putils::TCPListener server(4242);

    server += [&disconnected, &server]
    {
        auto &c = server.getLastClient();
        c.disconnected += [&disconnected] { disconnected = true; };
    };

    std::thread t([&server, &running]
                  {
                      while (running)
                          server.select({ 0, 0 });
                  }
    );

    {
        putils::TCPConnection client("127.0.0.1", 4242);
    }

    running = false;
    t.join();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    server.select({ 0, 0 }); // One more to make sure servers notices disconnection
    EXPECT_TRUE(disconnected);
}
