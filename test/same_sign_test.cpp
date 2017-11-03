#include "gtest/gtest.h"
#include "same_sign.hpp"

TEST(SameSignTest, Positive)
{
    EXPECT_TRUE(putils::sameSign(21, 42));
}

TEST(SameSignTest, Negative)
{
    EXPECT_TRUE(putils::sameSign(-21, -42));
}

TEST(SameSignTest, PositiveAndNegative)
{
    EXPECT_FALSE(putils::sameSign(-21, 42));
    EXPECT_FALSE(putils::sameSign(42, -21));
}

TEST(SameSignTest, PositiveAndZero)
{
    EXPECT_TRUE(putils::sameSign(0, 42));
    EXPECT_TRUE(putils::sameSign(42, 0));
}

TEST(SameSignTest, NegativeAndZero)
{
    EXPECT_TRUE(putils::sameSign(0, -42));
    EXPECT_TRUE(putils::sameSign(-42, 0));
}