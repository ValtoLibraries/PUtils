#include "gtest/gtest.h"
#include "casts.hpp"

struct CastsTest : public testing::Test
{
    struct A { virtual bool f() { return false; } };
    struct B : public A { bool f() final { return true; } };
};

TEST_F(CastsTest, StaticUniquePtrCast)
{
    auto ptr = std::unique_ptr<A>(new B);
    auto child = putils::static_unique_ptr_cast<B>(std::move(ptr));
    EXPECT_TRUE(child->f());
}

TEST_F(CastsTest, DynamicUniquePtrCastSucceeds)
{
    auto ptr = std::unique_ptr<A>(new B);
    auto child = putils::dynamic_unique_ptr_cast<B>(std::move(ptr));
    EXPECT_NE(child, nullptr);
}

TEST_F(CastsTest, DynamicUniquePtrCastFailes)
{
    auto ptr = std::make_unique<A>();
    auto child = putils::dynamic_unique_ptr_cast<B>(std::move(ptr));
    EXPECT_EQ(child, nullptr);
}