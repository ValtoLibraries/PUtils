#include "gtest/gtest.h"
#include "json.hpp"

struct JsonTest : testing::Test
{
    putils::json::Object o = putils::json::lex(
            putils::concat(
                    "{ obj: { key: \"value\" }, ",
                    "objectList: ",
                    "[ ",
                    "{ first: { key: \"value\" } }"
                            "{ second: { key: value } }",
                    " ], ",
                    "valueList: [ first, second ]",
                    " }"
            )
    );
};

TEST_F(JsonTest, PrettyPrint)
{
    const auto str = putils::json::prettyPrint(
            "{ obj: { key: value }, list: [ first: { key: value }, second: { key: value } ] }"
    );
    EXPECT_EQ(str,
              "{\n"
                      "\tobj: {\n"
                      "\t\tkey: value \n"
                      "\t},\n"
                      "\tlist: [\n"
              "\t\tfirst: {\n"
              "\t\t\tkey: value \n"
              "\t\t},\n"
              "\t\tsecond: {\n"
              "\t\t\tkey: value \n"
              "\t\t}\n"
              "\t]\n"
              "}"
    );
}

TEST_F(JsonTest, LexType)
{
    EXPECT_EQ(o.type, putils::json::Object::Type::Object);
}

TEST_F(JsonTest, LexObjectField)
{
    const auto &obj = o["obj"];
    EXPECT_EQ(obj.type, putils::json::Object::Type::Object);
    EXPECT_EQ(obj.fields.size(), 1);
    EXPECT_EQ(obj["key"].type, putils::json::Object::Type::Value);
    EXPECT_EQ(obj["key"].toString(), "value");
}

TEST_F(JsonTest, LexObjectListField)
{
    const auto &list = o["objectList"];
    EXPECT_EQ(list.type, putils::json::Object::Type::Array);
    EXPECT_EQ(list.items.size(), 2);

    const auto &obj = list[0];
    EXPECT_EQ(obj.type, putils::json::Object::Type::Object);
    EXPECT_EQ(obj.fields.size(), 1);
    const auto &field = obj["first"];
    EXPECT_EQ(field.type, putils::json::Object::Type::Object);
    EXPECT_EQ(field.fields.size(), 1);
    EXPECT_EQ(field["key"].type, putils::json::Object::Type::Value);
    EXPECT_EQ(field["key"].toString(), "value");

    const auto &obj2 = list[1];
    EXPECT_EQ(obj2.type, putils::json::Object::Type::Object);
    EXPECT_EQ(obj2.fields.size(), 1);
    const auto &field2 = obj2["second"];
    EXPECT_EQ(field2.type, putils::json::Object::Type::Object);
    EXPECT_EQ(field2["key"].type, putils::json::Object::Type::Value);
    EXPECT_EQ(field2["key"].toString(), "value");
}

TEST_F(JsonTest, LexValueListField)
{
    const auto &list = o["valueList"];
    EXPECT_EQ(list.type, putils::json::Object::Type::Array);
    EXPECT_EQ(list.items.size(), 2);

    const auto &v = list[0];
    EXPECT_EQ(v.type, putils::json::Object::Type::Value);
    EXPECT_EQ(v.toString(), "first");

    const auto &v2 = list[1];
    EXPECT_EQ(v2.type, putils::json::Object::Type::Value);
    EXPECT_EQ(v2.toString(), "second");
}
