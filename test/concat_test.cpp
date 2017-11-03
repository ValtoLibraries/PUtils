#include "gtest/gtest.h"
#include "concat.hpp"

TEST(ConcatTest, NoArgument)
{
    const auto str = putils::concat();
    EXPECT_EQ(str, "");
}

TEST(ConcatTest, OneArgument)
{
    const auto str = putils::concat("content");
    EXPECT_EQ(str, "content");
}

TEST(ConcatTest, MultipleArguments)
{
    const auto str = putils::concat("content", 42, "more");
    EXPECT_EQ(str, "content42more");
}
