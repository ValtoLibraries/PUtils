#include "gtest/gtest.h"
#include "RAII.hpp"

TEST(RAIITest, Release) {
    int i = 0;
    {
        putils::RAII<int &> obj(i, [](int & x) { x = 42; });
    }
    EXPECT_EQ(i, 42);
}

TEST(RAIITest, Move) {
    testing::internal::CaptureStdout();

    {
        putils::RAII<int> obj(42, [](auto && ...) { std::cout << 42; });
        putils::RAII<int> replacer(std::move(obj));
    }

    const auto str = testing::internal::GetCapturedStdout();
    EXPECT_EQ(str, "42");
}