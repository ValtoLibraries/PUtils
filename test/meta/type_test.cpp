#include "gtest/gtest.h"
#include "meta/type.hpp"

TEST(TypeTest, Wrapped)
{
    constexpr bool res = std::is_same<pmeta::type<int>::wrapped, int>::value;
    EXPECT_TRUE(res);
}

TEST(TypeTest, IndexScalar)
{
    EXPECT_EQ(pmeta::type<int>::index, pmeta::type<int>::index);
    EXPECT_NE(pmeta::type<int>::index, pmeta::type<double>::index);
}

TEST(TypeTest, IndexCustom)
{
    struct A {};
    struct B {};

    EXPECT_EQ(pmeta::type<A>::index, pmeta::type<A>::index);
    EXPECT_NE(pmeta::type<A>::index, pmeta::type<B>::index);
}
