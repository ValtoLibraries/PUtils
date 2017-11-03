#include "gtest/gtest.h"
#include "meta/table.hpp"

struct TableTest : testing::Test
{
    static auto &getTable()
    {
        static auto table = pmeta::make_table(
                "key", 0,
                42, "forty-two"
        );
        return table;
    }

    static auto &getTypeTable()
    {
        static auto table = pmeta::make_table(
                "string", pmeta::type<std::string>(),
                "int", pmeta::type<int>(),
                "float", pmeta::type<float>()
        );
        return table;
    }

    struct Checker
    {
        template<typename T>
        void operator()(pmeta::type<T>) { v = typeid(T).name(); }

        std::string v;
    };
};

TEST_F(TableTest, GetValue)
{
    int i;
    getTable().get_value("key", [&i](int value) { i = value; });
    EXPECT_EQ(i, 0);

    std::string v;
    getTable().get_value(42, [&v](const std::string &value) { v = value; });
    EXPECT_EQ(v, "forty-two");
}

TEST_F(TableTest, GetKey)
{
    std::string k;
    getTable().get_key(0, [&k](const std::string &key) { k = key; });
    EXPECT_EQ(k, "key");

    int i = 0;
    getTable().get_key("forty-two", [&i](int key) { i = key; });
    EXPECT_EQ(i, 42);
}

TEST_F(TableTest, GetTypeKey)
{
    std::string k;
    getTypeTable().get_key(pmeta::type<std::string>(), [&k](const std::string &key) { k = key; });
    EXPECT_EQ(k, "string");

    getTypeTable().get_key(pmeta::type<int>(), [&k](const std::string &key) { k = key; });
    EXPECT_EQ(k, "int");

    getTypeTable().get_key(pmeta::type<float>(), [&k](const std::string &key) { k = key; });
    EXPECT_EQ(k, "float");
}

TEST_F(TableTest, GetTypeValue)
{
    Checker checker;

    getTypeTable().get_value("string", checker);
    EXPECT_EQ(checker.v, typeid(std::string).name());

    getTypeTable().get_value("int", checker);
    EXPECT_EQ(checker.v, typeid(int).name());

    getTypeTable().get_value("float", checker);
    EXPECT_EQ(checker.v, typeid(float).name());
}