#include "gtest/gtest.h"
#include "CellularAutomaton.hpp"

struct CellularAutomatonTest : testing::Test
{
    putils::CellularAutomaton<bool> ca {
            {
                    { { 0 }, false },
                    { { 1 }, false },
                    { { 2 }, true },
                    { { 3 }, false },
                    { { 4 }, false }
            }
    };
};

TEST_F(CellularAutomatonTest, Step)
{
    const auto &cells = ca.step(
            [](const std::vector<const putils::Cell<bool> *> &neighbors, const putils::Cell<bool> &cell)
            {
                return std::find_if(neighbors.begin(), neighbors.end(), [](auto &&ptr){ return ptr->obj; })
                       != neighbors.end();
            },
            [](putils::Cell<bool> &cell)
            {
                cell.obj = !cell.obj;
            }
    );

    EXPECT_EQ(cells.size(), 5);
    EXPECT_FALSE(cells[0].obj);
    EXPECT_FALSE(cells[4].obj);
    EXPECT_TRUE(cells[1].obj);
    EXPECT_TRUE(cells[2].obj);
    EXPECT_TRUE(cells[3].obj);
}