#pragma once

#include <cctype>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "chop.hpp"
#include "concat.hpp"

namespace putils
{
    namespace json
    {
        template<typename String>
        std::string prettyPrint(String &&str);

        struct Object
        {
            enum class Type
            {
                Value,
                Object,
                Array
            };
            std::string value;
            std::vector<Object> items;
            std::map<std::string, Object> fields;
            Type type;
            // if items.size() > 0, then I'm an array
            // if fields.size() == 0, then I'm a simple value

            // Array
            Object &operator[](std::size_t i) { return items[i]; }
            const Object &operator[](std::size_t i) const { return items[i]; }

            // Object
            const Object &operator[](std::string_view name) const { return fields.at(name.data()); }
            Object &operator[](std::string_view name) { return fields[name.data()]; }

            // Value
            operator const std::string &() const { return value; }
            const std::string &toString() const { return value; }
            int toInt() const { return std::stoi(value); }
            double toDouble() const { return std::stod(value); }
            bool toBool() const
            {
                std::stringstream s;
                s << std::boolalpha << value;
                bool ret;
                s >> ret;
                return ret;
            }

            friend std::ostream &operator<<(std::ostream &s, const Object &obj)
            {
                s << obj.value;
                return s;
            }
        };

        template<typename String>
        Object lex(String &&str);
    }
}

namespace putils
{
    namespace json
    {
        namespace detail
        {
            template<typename Ret>
            void printLineAndSkipSpace(std::istringstream &s, Ret &ret, std::size_t indent)
            {
                ret.append(1, '\n');
                ret.append(indent, '\t');
                while (s && std::isspace(s.peek()))
                    s.get();
            }
        }

        template<typename String>
        std::string prettyPrint(String &&str)
        {
            static std::string startBlock = "{[";
            static std::string endBlock = "}]";
            static std::string newLine = ",";
            static std::string needSpace = ":";

            std::istringstream s(FWD(str));

            std::string ret;
            std::size_t indent = 0;

            while (s)
            {
                const char c = s.get();
                if (c == -1)
                    break;

                if (c == '\\')
                {
                    ret.append(1, s.get());
                    continue;
                }

                if (endBlock.find(c) != std::string::npos)
                {
                    --indent;
                    detail::printLineAndSkipSpace(s, ret, indent);
                }

                if (c != '\n')
                    ret.append(1, c);
                if (needSpace.find(c) != std::string::npos)
                {
                    ret.append(1, ' ');
                    while (s && std::isspace(s.peek()))
                        s.get();
                }

                if (newLine.find(c) != std::string::npos)
                    detail::printLineAndSkipSpace(s, ret, indent);

                if (startBlock.find(c) != std::string::npos)
                {
                    ++indent;
                    detail::printLineAndSkipSpace(s, ret, indent);
                }
            }

            return ret;
        }

        namespace
        {
            std::string lexValue(std::istringstream &s);
            Object lexArray(std::istringstream &s);
            Object lexObject(std::istringstream &s);

            void readKeyValue(std::istringstream &s, Object &ret)
            {
                while (std::isspace(s.peek()))
                    s.get();

                if (s.peek() == ',')
                    return;

                std::string key = lexValue(s);
                ret.value += key;

                ret.value.append(1, s.get()); // Skip ':'

                while (std::isspace(s.peek()))
                    s.get();

                if (s.peek() == '{')
                    ret[key] = lexObject(s);
                else if (s.peek() == '[')
                    ret[key] = lexArray(s);
                else
                    ret[key] = Object{ lexValue(s), {}, {}, Object::Type::Value };

                ret.value += ret[key].value;
            }

            std::string lexValue(std::istringstream &s)
            {
                std::string ret;

                if (s.peek() == '"')
                {
                    s.get();
                    while (s.peek() != '"')
                    {
                        if (s.peek() == '\\')
                            s.get();
                        ret.append(1, s.get());
                    }
                    s.get();
                    return ret;
                }

                while (s)
                {
                    const char c = s.peek();
                    if (c == -1)
                        throw std::runtime_error("Unexpected EOF");

                    if (c == '\\')
                    {
                        s.get();
                        ret.append(1, s.get());
                        continue;
                    }

                    if (c == ':' || c == ',' || c == '}' || c == ']')
                        break;
                    ret.append(1, s.get());
                }

                ret = putils::chop(ret);
                if ((ret.front() == '\"' && ret.back() == '\"') ||
                        (ret.front() == '\'' && ret.back() == '\''))
                    ret = ret.substr(1, ret.size() - 2);

                return ret;
            }

            Object lexObject(std::istringstream &s)
            {
                Object ret;

                while (std::isspace(s.peek()))
                    s.get();

                if (s.peek() != '{')
                {
                    ret.type = Object::Type::Value;
                    // ret.value.append(1, s.get());
                    ret.value = lexValue(s);
                    return ret;
                }

                ret.type = Object::Type::Object;
                ret.value.append(1, s.get()); // Skip '{'
                while (s)
                {
                    while (std::isspace(s.peek()))
                        s.get();

                    const char c = s.peek();
                    if (c == -1 || !s)
                        throw std::runtime_error("Unexpected EOF");

                    if (c == '}')
                    {
                        ret.value.append(1, s.get());
                        break;
                    }

                    readKeyValue(s, ret);

                    while (std::isspace(s.peek()))
                        s.get();

                    if (s.peek() == ',')
                        ret.value.append(1, s.get());
                }

                return ret;
            }

            Object lexArray(std::istringstream &s)
            {
                Object ret;
                ret.type = Object::Type::Array;

                ret.value.append(1, s.get()); // Skip '['

                while (s)
                {
                    while (std::isspace(s.peek()))
                        s.get();

                    const char c = s.peek();
                    if (c == -1)
                        throw std::runtime_error("Unexpected EOF");
                    if (c == ']')
                    {
                        ret.value.append(1, s.get());
                        break;
                    }

                    if (c == '{')
                        ret.items.push_back(lexObject(s));
                    else if (c == '[')
                        ret.items.push_back(lexArray(s));
                    else
                        ret.items.push_back(Object{ lexValue(s), {}, {}, Object::Type::Value });

                    ret.value += ret.items.back().value;

                    while (std::isspace(s.peek()))
                        s.get();

                    if (s.peek() == ',')
                        ret.value.append(1, s.get());
                }

                return ret;
            }
        }

        template<typename Str>
        Object lex(Str &&str)
        {
            std::istringstream s(FWD(str));

            while (std::isspace(s.peek()))
                s.get();

            if (s.peek() == '{')
                return lexObject(s);
            if (s.peek() == '[')
                return lexArray(s);
            throw std::runtime_error(concat("Unexpected character " + s.peek()));
        }
    }

#include <assert.h>

    namespace test
    {
        namespace json
        {
            inline void prettyPrint()
            {
                std::cout <<
                          putils::json::prettyPrint("{ obj: { key: value }, list: [ first: { key: value }, second: { key: value } ] }")
                          << std::endl;
            }

            inline void lex()
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
                assert(o.type == putils::json::Object::Type::Object);
                assert(o.fields.size() == 3);

                auto &obj = o.fields["obj"];
                assert(obj.type == putils::json::Object::Type::Object);
                assert(obj.fields.size() == 1);
                assert(obj.fields["key"].type == putils::json::Object::Type::Value);
                assert(obj.fields["key"].value == "value");

                auto &objectList = o.fields["objectList"];
                assert(objectList.type == putils::json::Object::Type::Array);
                assert(objectList.items.size() == 2);
                auto &object0 = objectList.items[0];
                assert(object0.type == putils::json::Object::Type::Object);
                assert(object0.fields.size() == 1);
                auto &first = object0.fields["first"];
                assert(first.type == putils::json::Object::Type::Object);
                assert(first.fields.size() == 1);
                assert(first.fields["key"].type == putils::json::Object::Type::Value);
                assert(first.fields["key"].value == "value");
                auto &object1 = objectList.items[1];
                assert(object1.type == putils::json::Object::Type::Object);
                assert(object1.fields.size() == 1);
                auto &second = object1.fields["second"];
                assert(second.type == putils::json::Object::Type::Object);
                assert(second.fields["key"].type == putils::json::Object::Type::Value);
                assert(second.fields["key"].value == "value");

                auto &valueList = o.fields["valueList"];
                assert(valueList.type == putils::json::Object::Type::Array);
                assert(valueList.items.size() == 2);
                auto &firstValue = valueList.items[0];
                assert(firstValue.type == putils::json::Object::Type::Value);
                assert(firstValue.value == "first");
                auto &secondValue = valueList.items[1];
                assert(secondValue.type == putils::json::Object::Type::Value);
                assert(secondValue.value == "second");
            }
        }
    }
}