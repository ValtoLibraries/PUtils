#include "gtest/gtest.h"
#include "meta/for_each.hpp"

TEST(TupleForEach, ForEach)
{
    int sum = 0;
    pmeta::tuple_for_each(
            std::make_tuple(42, 21),
            [&sum](auto i)
            {
                sum += i;
            }
    );

    EXPECT_EQ(sum, 63);
}

TEST(TupleForEach, ForEachRef)
{
    int i = 0;
    double d = 0;

    pmeta::tuple_for_each(
            std::make_tuple(std::ref(i), std::ref(d)),
            [&i, &d](auto &&v)
            {
                if constexpr (std::is_same<pmeta_typeof(v), int>::value)
                    i = 42;
                else if constexpr (std::is_same<pmeta_typeof(v), double>::value)
                    d = 84;
                else
                {
                    i = -42;
                    d = -42;
                }
            }
    );

    EXPECT_EQ(i, 42);
    EXPECT_EQ(d, 84);
}

TEST(TupleForEach, ForEachType)
{
    int i = 0;
    double d = 0;

    pmeta::tuple_for_each(
            std::tuple<pmeta::type<int>, pmeta::type<double>>(),
            [&i, &d](auto &&t)
            {
                if constexpr (std::is_same<pmeta_wrapped(t), int>::value)
                    i = 42;
                else if constexpr (std::is_same<pmeta_wrapped(t), double>::value)
                    d = 84;
                else
                {
                    i = -42;
                    d = -42;
                }
            }
    );

    EXPECT_EQ(i, 42);
    EXPECT_EQ(d, 84);
}
