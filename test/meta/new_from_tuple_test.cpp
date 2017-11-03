#include "gtest/gtest.h"
#include "meta/new_from_tuple.hpp"

struct NewFromTupleTest : testing::Test
{
    struct Obj
    {
        Obj(int i, double d, const std::string &str) : i(i), d(d), str(str) {}
        Obj(double d) : i(21), d(d), str("message") {}
        Obj() = default;

        int i = 0;
        double d = 0;
        std::string str;
    };
};

TEST_F(NewFromTupleTest, Normal)
{
    auto pod = pmeta::new_from_tuple<Obj>(std::make_tuple(42, 84.42, "string"));
    ASSERT_NE(pod, nullptr);
    EXPECT_EQ(pod->i, 42);
    EXPECT_EQ(pod->d, 84.42);
    EXPECT_EQ(pod->str, "string");
}

TEST_F(NewFromTupleTest, OneParameter)
{
    auto pod = pmeta::new_from_tuple<Obj>(std::make_tuple(84.42));
    ASSERT_NE(pod, nullptr);
    EXPECT_EQ(pod->i, 21);
    EXPECT_EQ(pod->d, 84.42);
    EXPECT_EQ(pod->str, "message");
}

TEST_F(NewFromTupleTest, NoParameter)
{
    auto pod = pmeta::new_from_tuple<Obj>(std::make_tuple());
    ASSERT_NE(pod, nullptr);
    EXPECT_EQ(pod->i, 0);
    EXPECT_EQ(pod->d, 0);
    EXPECT_EQ(pod->str, "");
}
