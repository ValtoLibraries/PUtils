#include "gtest/gtest.h"
#include "read_stream.hpp"

TEST(ReadStreamTest, Test)
{
    const auto str = "One two three ooooh yay\nSecond line";
    std::stringstream s(str);
    EXPECT_EQ(putils::read_stream(s), str);
}