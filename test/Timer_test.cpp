#include <thread>
#include "gtest/gtest.h"
#include "Timer.hpp"

struct TimerTest : testing::Test
{
    putils::Timer t{ std::chrono::milliseconds(5) };
};

using namespace std::chrono_literals;

TEST_F(TimerTest, IsDone)
{
    EXPECT_FALSE(t.isDone());
    std::this_thread::sleep_for(5ms);
    EXPECT_TRUE(t.isDone());
}

TEST_F(TimerTest, GetTimesDone)
{
    EXPECT_LT(t.getTimesDone(), 1);
    std::this_thread::sleep_for(5ms);
    EXPECT_GE(t.getTimesDone(), 1);
    std::this_thread::sleep_for(5ms);
    EXPECT_GE(t.getTimesDone(), 2);
}

TEST_F(TimerTest, GetTimeSinceStart)
{
    EXPECT_LT(t.getTimeSinceStart(), 1ms);
    std::this_thread::sleep_for(5ms);
    EXPECT_GE(t.getTimeSinceStart(), 5ms);
}

TEST_F(TimerTest, GetTimeSinceDone)
{
    std::this_thread::sleep_for(5ms);
    EXPECT_LE(t.getTimeSinceDone(), 1ms);
    std::this_thread::sleep_for(5ms);
    EXPECT_GE(t.getTimeSinceDone(), 5ms);
}

TEST_F(TimerTest, GetTimeLeft)
{
    EXPECT_LE(t.getTimeLeft(), 5ms);
    std::this_thread::sleep_for(2ms);
    EXPECT_LE(t.getTimeLeft(), 3ms);
}

TEST_F(TimerTest, Restart)
{
    std::this_thread::sleep_for(5ms);
    t.restart();
    EXPECT_FALSE(t.isDone());
}
