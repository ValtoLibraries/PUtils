#include "gtest/gtest.h"
#include "AStar.hpp"

struct AStarTest : testing::Test
{
    const std::vector<putils::Point2i> steps = putils::AStar::getNextDirection<int>(
            putils::Point2i{ 0, 0 }, putils::Point2i{ 3, 0 }, true, 1, 1,
            [](const putils::Point2i &, const putils::Point2i &) { return true; }
    );

    putils::Point2i first{ 1, 0 };
    putils::Point2i second{ 2, 0 };
    putils::Point2i third{ 3, 0 };
};

TEST_F(AStarTest, FirstStep)
{
    EXPECT_EQ(steps[0], first);
}

TEST_F(AStarTest, StraightLine)
{
    EXPECT_EQ(steps.size(), 3);
    EXPECT_EQ(steps[0], first);
    EXPECT_EQ(steps[1], second);
    EXPECT_EQ(steps[2], third);
}

TEST_F(AStarTest, Obstacle)
{
    const std::vector<putils::Point2i> msteps = putils::AStar::getNextDirection<int>(
            putils::Point2i{ 0, 0 }, putils::Point2i{ 3, 0 }, true, 1, 1,
            [](const putils::Point2i &, const putils::Point2i &to) { return to != putils::Point2i{ 2, 0 }; }
    );

    EXPECT_EQ(msteps.size(), 3);
    EXPECT_EQ(msteps[0], first);

    putils::Point2i top{ 2, -1 };
    putils::Point2i bottom{ 2, -1 };
    EXPECT_TRUE(msteps[1] == top || msteps[1] == bottom)
                        << "\tmsteps[1]: " << msteps[1] << std::endl
                        << "\ttop:" << top << std::endl
                        << "\tbottom" << bottom << std::endl;

    EXPECT_EQ(msteps[2], third);
}
