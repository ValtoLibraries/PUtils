#include "gtest/gtest.h"
#include "sign.hpp"

TEST(SignTest, Positive)
{
    EXPECT_EQ(putils::sign(42), 1);
}

TEST(SignTest, Negative)
{
    EXPECT_EQ(putils::sign(-42), -1);
}

TEST(SignTest, Zero)
{
    EXPECT_EQ(putils::sign(0), 0);
}
