#include "gtest/gtest.h"
#include "meta/nameof.hpp"

TEST(NameOfTest, Normal)
{
    EXPECT_STREQ(pmeta_nameof(normalTest), "normalTest");
}

TEST(NameOfTest, Private)
{
    EXPECT_STREQ(pmeta_nameof_private(_privateTest).data(), "privateTest");
}