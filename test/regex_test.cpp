#include "gtest/gtest.h"
#include "regex.hpp"

TEST(RegexTest, Comparison)
{
    EXPECT_TRUE("test" == std::regex("t.*"));
}

TEST(RegexTest, Match)
{
    const std::cmatch matches = ("test" == std::make_pair(std::regex("t(.*)t"), true));
    EXPECT_EQ(matches.size(), 2);
    EXPECT_EQ(matches[0], "test");
    EXPECT_EQ(matches[1], "es");
}

TEST(RegexTest, LiteralComparison)
{
    EXPECT_TRUE("test" == "t.*"_r);
}

TEST(RegexTest, LiteralMatch)
{
    const std::cmatch matches = ("test" == "t(.*)t"_m);
    EXPECT_EQ(matches.size(), 2);
    EXPECT_EQ(matches[0], "test");
    EXPECT_EQ(matches[1], "es");
}
