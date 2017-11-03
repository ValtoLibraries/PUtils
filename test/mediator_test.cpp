#include "gtest/gtest.h"
#include "Mediator.hpp"
#include "Module.hpp"

struct MediatorTest : testing::Test
{
    struct Example
    {
        struct Query
        {
            std::string msg;
            putils::BaseModule *sender;
        };

        struct Response
        {
            std::string msg;
        };
    };

    struct SenderModule : putils::Module<SenderModule, Example::Query>
    {
        void work(std::string_view s)
        {
            send(std::string(s));
        }

        void handle(const Example::Query &q)
        {
            sendTo(Example::Response{ q.msg }, *q.sender);
        }
    };

    struct ReceiverModule : putils::Module<ReceiverModule, std::string>
    {
        void work(std::string_view s)
        {
            msg = query<Example::Response>(Example::Query{ std::string(s) }).msg;
        }

        void handle(const std::string &str)
        {
            received = true;
            msg = str;
        }

        bool received = false;
        std::string msg;
    };

    SenderModule sender;
    SenderModule sender2;
    ReceiverModule receiver;
    ReceiverModule receiver2;
    putils::Mediator mediator;

    MediatorTest()
    {
        mediator.addModule(sender);
        mediator.addModule(sender2);
        mediator.addModule(receiver);
        mediator.addModule(receiver2);
    }
};

TEST_F(MediatorTest, OneReceiver)
{
    sender.work("Message");
    EXPECT_TRUE(receiver.received);
    EXPECT_EQ(receiver.msg, "Message");
}

TEST_F(MediatorTest, SeveralReceivers)
{
    sender.work("Message");
    EXPECT_TRUE(receiver.received);
    EXPECT_EQ(receiver.msg, "Message");
    EXPECT_TRUE(receiver2.received);
    EXPECT_EQ(receiver2.msg, "Message");
}

TEST_F(MediatorTest, SeveralSenders)
{
    sender.work("Message");
    EXPECT_TRUE(receiver.received);
    EXPECT_EQ(receiver.msg, "Message");
    EXPECT_TRUE(receiver2.received);
    EXPECT_EQ(receiver2.msg, "Message");
    sender2.work("New");
    EXPECT_TRUE(receiver.received);
    EXPECT_EQ(receiver.msg, "New");
    EXPECT_TRUE(receiver2.received);
    EXPECT_EQ(receiver2.msg, "New");
}

TEST_F(MediatorTest, Query)
{
    receiver.work("Message");
    EXPECT_EQ(receiver.msg, "Message");
}
