#include "gtest/gtest.h"
#include "meta/GenLinearHierarchy.hpp"

struct GenLinearHierarchyTest : testing::Test
{
    template<typename T>
    struct Handle
    {
        bool handle(T) { return true; }
    };

    struct Example : pmeta::GenLinearHierarchy<Handle, int, std::string>
    {
        template<typename T>
        bool handle(T obj) { return static_cast<Handle<T> &>(*this).handle(obj); }
    };
};

TEST_F(GenLinearHierarchyTest, Test)
{
    int i = 42;
    std::string s;
    EXPECT_TRUE(Example().handle(i));
    EXPECT_TRUE(Example().handle(s));
}