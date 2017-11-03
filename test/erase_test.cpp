#include <unordered_map>
#include <vector>
#include <list>
#include <bits/unordered_map.h>
#include "gtest/gtest.h"
#include "erase.hpp"

TEST(EraseTest, Vector)
{
    std::vector<int> v { 1, 2, 3 };
    putils::erase(v, 2);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 3);
}

TEST(EraseTest, BadValue)
{
    std::vector<int> v { 1 };
    putils::erase(v, 2);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 1);
}

TEST(EraseTest, List)
{
    std::list<int> v { 1, 2, 3 };
    putils::erase(v, 2);
    EXPECT_EQ(v.size(), 2);
    auto it = v.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 3);
}
