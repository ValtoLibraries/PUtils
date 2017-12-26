#include "gtest/gtest.h"
#include "chop.hpp"

TEST(ChopTest, Ignore)
{
    EXPECT_EQ(putils::chop("test"), "test");
}

TEST(ChopTest, Space)
{
    EXPECT_EQ(putils::chop("test "), "test");
}

TEST(ChopTest, Tab)
{
    EXPECT_EQ(putils::chop("test\t"), "test");
}

TEST(ChopTest, Newline)
{
    EXPECT_EQ(putils::chop("test\n"), "test");
}

TEST(ChopTest, CarriageReturn)
{
    EXPECT_EQ(putils::chop("test\r"), "test");
}

TEST(ChopTest, All)
{
    EXPECT_EQ(putils::chop("test\r \n\t"), "test");
}

