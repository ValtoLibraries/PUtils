#include "gtest/gtest.h"
#include "meta/has_type.hpp"

TEST(HasTypeTest, Good)
{
    constexpr bool ret = pmeta::has_type<int, double, char, int>::value;
    EXPECT_TRUE(ret);
}

TEST(HasTypeTest, Bad)
{
    constexpr bool ret = pmeta::has_type<int, double, char>::value;
    EXPECT_FALSE(ret);
}

TEST(HasTypeTest, Empty)
{
    EXPECT_FALSE(pmeta::has_type<int>::value);
}