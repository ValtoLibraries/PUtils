#include "gtest/gtest.h"
#include "QuadTree.hpp"

struct QuadTreeTest : testing::Test
{
    putils::QuadTree<int> tree{ { { 0, 0 }, { 64, 64 } } };
};

TEST_F(QuadTreeTest, Add)
{
    tree.add(42, { { 0, 0, }, { 1, 1 } });
    const auto &objects = tree.query({ { 0, 0 }, { 1, 1 } });
    EXPECT_EQ(objects.size(), 1);
    EXPECT_EQ(objects[0], 42);
}

TEST_F(QuadTreeTest, Remove)
{
    tree.add(42, { { 0, 0, }, { 1, 1 } });
    tree.remove(42);
    const auto &objects = tree.query({ { 0, 0 }, { 1, 1 } });
    EXPECT_EQ(objects.size(), 0);
}

TEST_F(QuadTreeTest, Move)
{
    tree.add(42, { { 0, 0, }, { 1, 1 } });
    tree.move(42, { { 10, 10 }, { 1, 1 } });
    const auto &objects = tree.query({ { 0, 0 }, { 1, 1 } });
    EXPECT_EQ(objects.size(), 0);

    const auto &other = tree.query({ { 10, 10 }, { 1, 1 } });
    EXPECT_EQ(other.size(), 1);
    EXPECT_EQ(other[0], 42);
}

TEST_F(QuadTreeTest, LargeData)
{
    int id = 0;
    for (int x = 0; x < 64; ++x)
        for (int y = 0; y < 64; ++y)
            tree.add(++id, { { x, y }, { 1, 1 } });

    const auto all = tree.query({ { 0, 0 }, { 64, 64 } });
    EXPECT_EQ(all.size(), 4096);

    tree.add(4242, { { 3, 3 }, { 2, 2 } });

    const auto objects = tree.query({ { 0, 0 }, { 4, 4 } });
    EXPECT_EQ(objects.size(), 17);
    EXPECT_NE(std::find_if(objects.begin(), objects.end(), [](auto &&obj) { return obj == 4242; }), objects.end());
}
