#include "gtest/gtest.h"
#include "reflection/Reflectible.hpp"

struct ReflectibleTest : testing::Test
{
    class ExampleParent
    {
    };

    class Test : public putils::Reflectible<Test>, public ExampleParent
    {
    public:
        Test()
        {
        }

    public:
        std::string exampleMethod() const { return "Method"; }
        std::string _exampleAttribute = "Attribute";

    public:
        static const auto get_class_name() { return "Test"; }

        static const auto &get_attributes()
        {
            static const auto table = pmeta::make_table(
                    pmeta_reflectible_attribute_private(&Test::_exampleAttribute)
            );
            return table;
        }

        static const auto &get_methods()
        {
            static const auto table = pmeta::make_table(
                    pmeta_reflectible_attribute(&Test::exampleMethod)
            );
            return table;
        }

        static const auto &get_parents()
        {
            static const auto table = pmeta::make_table(
                    pmeta_reflectible_parent(ExampleParent)
            );
            return table;
        }
    };
};

TEST_F(ReflectibleTest, ReflectibleAttribute)
{
    const auto p = std::make_pair(pmeta_reflectible_attribute(&Test::exampleMethod));
    EXPECT_EQ(p.first, "exampleMethod");
    EXPECT_EQ(p.second, &Test::exampleMethod);
}

TEST_F(ReflectibleTest, ReflectibleAttributePrivate)
{
    const auto p = std::make_pair(pmeta_reflectible_attribute_private(&Test::_exampleAttribute));
    EXPECT_EQ(p.first, "exampleAttribute");
    EXPECT_EQ(p.second, &Test::_exampleAttribute);
}

TEST_F(ReflectibleTest, IsReflectible)
{
    EXPECT_TRUE(putils::is_reflectible<Test>::value);
    EXPECT_FALSE(putils::is_reflectible<int>::value);
}

TEST_F(ReflectibleTest, GetClassName)
{
    EXPECT_STREQ(Test::get_class_name(), pmeta_nameof(Test));
}

TEST_F(ReflectibleTest, GetAttributes)
{
    std::string name;
    Test t;

    pmeta::tuple_for_each(
            Test::get_attributes().getKeyValues(),
            [&name, &t](auto &&p)
            {
                name = p.first;
                t.*p.second = "Changed";
            }
    );

    EXPECT_EQ(name, pmeta_nameof_private(_exampleAttribute));
    EXPECT_EQ(t._exampleAttribute, "Changed");
}

TEST_F(ReflectibleTest, GetMethods)
{
    std::string name;
    std::string res;
    Test t;

    pmeta::tuple_for_each(
            Test::get_methods().getKeyValues(),
            [&name, &res, &t](auto &&p)
            {
                name = p.first;
                res = (t.*p.second)();
            }
    );

    EXPECT_EQ(name, pmeta_nameof(exampleMethod));
    EXPECT_EQ(res, "Method");
}

TEST_F(ReflectibleTest, GetParents)
{
    std::string name;
    bool found = false;

    pmeta::tuple_for_each(
            Test::get_parents().getKeyValues(),
            [&name, &found](auto &&p)
            {
                using Type = pmeta_wrapped(p.second);
                if constexpr (std::is_same<Type, ExampleParent>::value)
                    found = true;
                name = p.first;
            }
    );

    EXPECT_EQ(name, pmeta_nameof(ExampleParent));
    EXPECT_TRUE(found);
}
