#include "gtest/gtest.h"
#include "to_string.hpp"

TEST(ToStringTest, String)
{
    EXPECT_EQ(putils::to_string("str"), "str");
}

TEST(ToStringTest, Int)
{
    EXPECT_EQ(putils::to_string(42), "42");
}

TEST(ToStringTest, Bool)
{
    EXPECT_EQ(putils::to_string(true), "true");
}