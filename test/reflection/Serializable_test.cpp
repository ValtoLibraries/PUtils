#include "gtest/gtest.h"
#include "reflection/Serializable.hpp"

struct SerializableTest : testing::Test
{
    struct T : putils::Serializable<T>
    {
        T() : putils::Serializable<T>(std::make_pair(pmeta_reflectible_attribute(&T::x))) {}
        int x = 42;
    };
};

TEST_F(SerializableTest, Serialize)
{
    EXPECT_EQ(putils::to_string(T()), "{x: 42}");
}

TEST_F(SerializableTest, Unserialize)
{
    T obj;
    std::stringstream s;
    s << "{x: 84}";
    s >> obj;
    EXPECT_EQ(obj.x, 84);
}

TEST_F(SerializableTest, UnserializeWhiteSpace)
{
    T obj;
    std::stringstream s;
    s << "   {    x    :  84   } ";
    s >> obj;
    EXPECT_EQ(obj.x, 84);
}

TEST_F(SerializableTest, MultipleFields)
{
    struct M : putils::Serializable<M>
    {
        M() : putils::Serializable<M>(
                std::make_pair(pmeta_reflectible_attribute(&M::x)),
                std::make_pair(pmeta_reflectible_attribute(&M::y))
        ) {}
        int x = 42;
        int y = 84;
    };

    EXPECT_EQ(putils::to_string(M()), "{x: 42, y: 84}");
}

TEST_F(SerializableTest, UnserializeMultipleFields)
{
    struct M : putils::Serializable<M>
    {
        M() : putils::Serializable<M>(
                std::make_pair(pmeta_reflectible_attribute(&M::x)),
                std::make_pair(pmeta_reflectible_attribute(&M::y))
        ) {}
        int x = 42;
        int y = 84;
    };

    M obj;
    std::stringstream s;
    s << "{x: 84, y: 42 }";
    s >> obj;
    EXPECT_EQ(obj.x, 84);
    EXPECT_EQ(obj.y, 42);
}

TEST_F(SerializableTest, Vector)
{
    struct M : putils::Serializable<M>
    {
        M() : putils::Serializable<M>(std::make_pair(pmeta_reflectible_attribute(&M::x))) {}
        std::vector<int> x = { 0, 1, 2, 3 };
    };

    EXPECT_EQ(putils::to_string(M()), "{x: [0,1,2,3]}");
}

TEST_F(SerializableTest, List)
{
    struct M : putils::Serializable<M>
    {
        M() : putils::Serializable<M>(std::make_pair(pmeta_reflectible_attribute(&M::x))) {}
        std::list<int> x = { 0, 1, 2, 3 };
    };

    EXPECT_EQ(putils::to_string(M()), "{x: [0,1,2,3]}");
}

TEST_F(SerializableTest, Ptr)
{
    struct M : putils::Serializable<M>
    {
        M() : putils::Serializable<M>(std::make_pair(pmeta_reflectible_attribute(&M::x))) {}
        int *x = new int(42);
    };

    EXPECT_EQ(putils::to_string(M()), "{x: 42}");
}

TEST_F(SerializableTest, UnserializePtr)
{
    struct M : putils::Serializable<M>
    {
        M() : putils::Serializable<M>(std::make_pair(pmeta_reflectible_attribute(&M::x))) {}
        int *x = new int(42);
    };

    M obj;
    std::stringstream s;
    s << "{x: 21}";
    s >> obj;
    EXPECT_EQ(*obj.x, 21);
}

TEST_F(SerializableTest, UniquePtr)
{
    struct M : putils::Serializable<M>
    {
        M() : putils::Serializable<M>(std::make_pair(pmeta_reflectible_attribute(&M::x))) {}
        std::unique_ptr<int> x { new int(42) };
    };

    EXPECT_EQ(putils::to_string(M()), "{x: 42}");
}

TEST_F(SerializableTest, SharedPtr)
{
    struct M : putils::Serializable<M>
    {
        M() : putils::Serializable<M>(std::make_pair(pmeta_reflectible_attribute(&M::x))) {}
        std::shared_ptr<int> x { new int(42) };
    };

    EXPECT_EQ(putils::to_string(M()), "{x: 42}");
}

TEST_F(SerializableTest, Map)
{
    struct M : putils::Serializable<M>
    {
        M() : putils::Serializable<M>(std::make_pair(pmeta_reflectible_attribute(&M::x))) {}
        std::map<std::string, int> x { { "one", 1 }, { "two", 2 }, { "three", 3 } };
    };

    EXPECT_EQ(putils::to_string(M()), "{x: {one: 1, three: 3, two: 2}}");
}

TEST_F(SerializableTest, UnserializeMap)
{
    struct M : putils::Serializable<M>
    {
        M() : putils::Serializable<M>(std::make_pair(pmeta_reflectible_attribute(&M::x))) {}
        std::map<std::string, int> x { { "one", 1 }, { "two", 2 }, { "three", 3 } };
    };

    M obj;
    std::stringstream s;
    s << "{ x: { one: 2, two: 1 } }";
    s >> obj;
    EXPECT_EQ(obj.x["one"], 2);
    EXPECT_EQ(obj.x["two"], 1);
}

TEST_F(SerializableTest, UnorderedMap)
{
    struct M : putils::Serializable<M>
    {
        M() : putils::Serializable<M>(std::make_pair(pmeta_reflectible_attribute(&M::x))) {}
        std::unordered_map<std::string, int> x { { "one", 1 }, { "two", 2 }, { "three", 3 } };
    };

    EXPECT_EQ(putils::to_string(M()), "{x: {three: 3, two: 2, one: 1}}");
}

TEST_F(SerializableTest, Reflectible)
{
    struct M : putils::Reflectible<M>, putils::Serializable<M>
    {
        std::string str = "msg";
        int i = 42;

        static const auto get_class_name() { return "M"; }
        static const auto &get_attributes()
        {
            static const auto table = pmeta::make_table(
                    pmeta_reflectible_attribute(&M::str),
                    pmeta_reflectible_attribute(&M::i)
            );
            return table;
        }
        static void get_methods() {}
        static void get_parents() {}
    };

    EXPECT_EQ(putils::to_string(M()), "{str: \"msg\", i: 42}");
}
