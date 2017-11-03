#include "gtest/gtest.h"
#include "AbstractFactory.hpp"

struct AbstractFactoryTest : testing::Test
{
    class CFactory : public putils::AbstractFactory<int, double>
    {
        std::unique_ptr<int> makeImpl(pmeta::type<int>) noexcept final { return std::make_unique<int>(0); }
        std::unique_ptr<double> makeImpl(pmeta::type<double>) noexcept final { return std::make_unique<double>(42); }
    };

    CFactory factory;
};

TEST_F(AbstractFactoryTest, CreateFirstType)
{
    EXPECT_EQ(*(factory.make<int>()), 0);
}

TEST_F(AbstractFactoryTest, CreateSecondType)
{
    EXPECT_EQ(*(factory.make<double>()), 42);
}

