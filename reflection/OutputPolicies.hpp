#pragma once

#include <cctype>
#include <sstream>
#include <list>
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>
#include <type_traits>

#include "meta/type.hpp"
#include "to_string.hpp"
#include "json.hpp"
#include "meta/for_each.hpp"
#include "traits.hpp"

namespace putils {
    namespace OutputPolicies {
        struct Json {
            static void startSerialize(std::ostream & s) { s << '{'; }
            static void endSerialize(std::ostream & s) { s << '}'; }
            static void serializeNewField(std::ostream & s) { s << ", "; }

            template<typename T, typename Tuple>
            static void unserialize(std::istream & s, T & obj, const Tuple & tuple) {
                std::size_t openBraces = 0;
                std::string str;

                while (std::isspace(s.peek()))
                    s.get();

                while (s) {
                    const char c = s.get();
                    if (c == '{' || c == '[')
                        ++openBraces;
                    else if (c == '}' || c == ']')
                        --openBraces;

                    str.append(1, c);

                    if (!openBraces)
                        break;
                }

                const auto jsonObject = putils::json::parse(str);

                pmeta::tuple_for_each(tuple, [&s, &obj, &jsonObject](const auto & attr) {
					using MemberType = std::remove_reference_t<decltype(std::declval<T>().*(attr.second))>;

					if constexpr (!std::is_const<MemberType>::value && !std::is_abstract<MemberType>::value) {
						const auto it = jsonObject.find(std::string(attr.first));
						if (it == jsonObject.end())
							return;

						std::stringstream stream(it->dump());
						unserialize(stream, obj.*(attr.second));
					}
                });
            };

            /*
             * Bools
             */

            static void serialize(std::ostream & s, std::string_view name, bool value) {
                s << '"' << name << '"' << ": " << std::boolalpha << value << std::noboolalpha;
            }

            static void unserialize(std::istream & s, bool & attr) {
                /*
                                while (std::isspace(s.peek()))
                                    s.get();
                                while (s.get() != ':');
                                while (std::isspace(s.peek()))
                                    s.get();
                */

                std::string value;
                while (s && s.peek() != ',' && s.peek() != '}') {
                    const char c = s.get();
                    if (c == '\\')
                        value.append(1, s.get());
                    else
                        value.append(1, c);
                }

                std::stringstream stream;
                stream << std::boolalpha << putils::chop(value);
                stream >> attr;
            }

            /*
             * Maps
             */

            template<typename Map>
            static void printMap(std::ostream & s, std::string_view name, Map && map) {
                s << '"' << name << '"' << ": {";
                bool first = true;
                for (const auto & pair : map) {
                    if (!first)
                        s << ", ";

                    serialize(s, putils::to_string(pair.first), pair.second);
                    first = false;
                }
                s << "}";
            }

            template<typename Key, typename Value, typename Map>
            static void unserializeMap(std::istream & s, Map & map) {
                map.clear();
                /*
                                while (std::isspace(s.peek()))
                                    s.get();
                                while (s && s.get() != ':');
                                while (std::isspace(s.peek()))
                                    s.get();
                 */

                while (s && s.get() != '{');

                if (!s)
                    throw std::runtime_error("Bad format: expected '{'");

                while (s.peek() != '}') {
                    while (std::isspace(s.peek()))
                        s.get();

                    std::string key;
                    while (s.peek() != ':' && s.peek() != '}') {
                        const char c = s.get();
                        if (c == '\\')
                            key.append(1, s.get());
                        else
                            key.append(1, c);
                    }

                    Key k;
                    {
                        std::stringstream tmp(key);
                        unserialize(tmp, k);
                    }

                    if (s.get() != ':')
                        throw std::runtime_error("Bad format: expected ':'");

                    while (std::isspace(s.peek()))
                        s.get();

                    std::string value;
                    while (s.peek() != ',' && s.peek() != '}') {
                        const char c = s.get();
                        if (c == '\\')
                            value.append(1, s.get());
                        else
                            value.append(1, c);
                    }

                    if (s.peek() == ',')
                        s.get();

                    {
                        std::stringstream tmp(value);
                        unserialize(tmp, map[k]);
                    }
                }
				s.get(); // }
            }

            template<typename Key, typename Value>
            static void serialize(std::ostream & s, std::string_view name,
                                  const std::unordered_map<Key, Value> & map) { printMap(s, name, map); }
            template<typename Key, typename Value>
            static void unserialize(std::istream & s, std::unordered_map<Key, Value> & map) { unserializeMap<Key, Value>(s, map); }


            template<typename Key, typename Value>
            static void serialize(std::ostream & s, std::string_view name,
                                  const std::map<Key, Value> & map) { printMap(s, name, map); }
            template<typename Key, typename Value>
            static void unserialize(std::istream & s, std::map<Key, Value> & map) { unserializeMap<Key, Value>(s, map); }

            /*
             * Ptrs
             */

            template<typename Ptr>
            static void printPtr(std::ostream & s, std::string_view name, Ptr && ptr) { serialize(s, name, *ptr); }

            template<typename T>
            static void serialize(std::ostream & s, std::string_view name, const std::unique_ptr<T> & ptr) { printPtr(s, name, ptr); }
            template<typename T>
            static void unserialize(std::istream & s, std::unique_ptr<T> & ptr) {
				ptr = nullptr;
            }

            template<typename T>
            static void serialize(std::ostream & s, std::string_view name, const std::shared_ptr<T> & ptr) { printPtr(s, name, ptr); }
            template<typename T>
            static void unserialize(std::istream & s, std::shared_ptr<T> & ptr) {
				ptr = nullptr;
            }

            /*
             * Containers
             */

            template<typename Container>
            static void printContainer(std::ostream & s, std::string_view name, const Container & container) {
                s << '"' << name << '"' << ": [";
                bool first = true;
                for (const auto & val : container) {
					if constexpr (putils::is_streamable<std::ostream, pmeta_typeof(val)>::value) {
						if (!first)
							s << ",";

						if constexpr (std::is_constructible<std::string, decltype(val)>::value || std::is_pointer<pmeta_typeof(val)>::value)
							s << '"' << val << '"';
						else
							s << val;

						first = false;
					}
                }

                s << "]";
            }

            template<template<typename, typename> typename Container, typename T, typename _>
            static void unserializeContainer(std::istream & s, Container<T, _> & attr) {
                attr.clear();

                /*
                                while (std::isspace(s.peek()))
                                    s.get();
                                while (s.get() != ':');
                                while (std::isspace(s.peek()))
                                    s.get();
                */

                while (s.get() != '[');
                while (s.peek() != ']') {
                    T obj;
                    unserializeContainerElement(s, obj);
					if constexpr (!std::is_pointer<T>::value)
						attr.push_back(obj);
                    while (s.peek() != ']' && s.get() != ',');
                }

                s.get();
            }

            template<typename T>
            static void unserializeContainerElement(std::istream & s, T & attr) {
                std::string value;
                while (s.peek() != ',' && s.peek() != ']') {
                    const char c = s.get();
                    if (c == '\\')
                        value.append(1, s.get());
                    else
                        value.append(1, c);
                }
				std::stringstream tmp(putils::chop(value));
				unserialize(tmp, attr);
            }

            template<typename T>
            static void serialize(std::ostream & s, std::string_view name, const std::list<T> & container) { printContainer(s, name, container); }
            template<typename T>
            static void unserialize(std::istream & s, std::list<T> & attr) { unserializeContainer(s, attr); }

            template<typename T>
            static void serialize(std::ostream & s, std::string_view name, const std::vector<T> & container) { printContainer(s, name, container); }
            template<typename T>
            static void unserialize(std::istream & s, std::vector<T> & attr) { unserializeContainer(s, attr); }

            template<typename T>
            static void serialize(std::ostream & s, std::string_view name, const std::set<T> & container) { printContainer(s, name, container); }
            template<typename T>
            static void unserialize(std::istream & s, std::set<T> & attr) { unserializeContainer(s, attr); }

            template<typename T>
            static void serialize(std::ostream & s, std::string_view name, const std::unordered_set<T> & container) { printContainer(s, name, container); }
            template<typename T>
            static void unserialize(std::istream & s, std::unordered_set<T> & attr) { unserializeContainer(s, attr); }

            /*
             * "Normal" objects
             */

            template<typename T>
            static void serialize(std::ostream & s, std::string_view name, const T & attr) {
				if constexpr (std::is_enum<T>::value)
					s << '"' << name << '"' << ": " << (int)attr;
				else if constexpr (std::is_pointer<T>::value)
					printPtr(s, name, attr);
				else if constexpr (std::is_constructible<std::string_view, T>::value)
					s << '"' << name << '"' << ": \"" << attr << "\"";
				else if constexpr (putils::is_streamable<std::ostream, T>::value)
					s << '"' << name << '"' << ": " << attr;
				else
					s << '"' << name << '"' << ": " << '"' << &attr << '"';
            }

            template<typename T, typename = std::enable_if_t<std::is_enum<T>::value>>
            static void unserializeImpl(std::istream & s, T & attr, const char *) {
                unserialize(s, (int &) attr);
            }


            template<typename T, typename = std::enable_if_t<std::is_pointer<T>::value>>
            static void unserializeImpl(std::istream & s, T & attr, std::string_view) {
				attr = nullptr;
            }

            template<typename T, typename = std::enable_if_t<!std::is_enum<T>::value && !std::is_pointer<T>::value>>
            static void unserializeImpl(std::istream & s, T & attr, std::string) {
                /*
                                while (std::isspace(s.peek()))
                                    s.get();
                                while (s && s.get() != ':');
                                while (std::isspace(s.peek()))
                                    s.get();
                */

                std::string value;
                std::size_t openBraces = 0;
                while (s && s.peek() != -1) // && s.peek() != ',' && s.peek() != '}')
                {
                    if (s.peek() == '{')
                        ++openBraces;
                    else if (s.peek() == '}') {
                        if (!openBraces)
                            break;
                        else
                            --openBraces;
                    } else if (s.peek() == ',' && !openBraces)
                        break;

                    const char c = s.get();
                    if (c == '\\')
                        value.append(1, s.get());
                    else
                        value.append(1, c);
                }

				if constexpr (putils::is_unstreamable<std::stringstream, T>::value) {
					if (value[0] == '"')
						std::stringstream(putils::chop(value.substr(1, value.size() - 2))) >> attr;
					else
						std::stringstream(putils::chop(value)) >> attr;
				}
            }

            template<typename T>
            static void unserialize(std::istream & s, T & attr) {
                unserializeImpl(s, attr, "");
            }
        };


        struct Default {
            static void startSerialize(std::ostream & s) {}
            static void endSerialize(std::ostream & s) {}
            static void serializeNewField(std::ostream & s) {}

            template<typename T, typename Tuple>
            static void unserialize(std::istream & s, T & obj, const Tuple & tuple) {
                pmeta::tuple_for_each(tuple, [&s, &obj](const auto & attr) {
                    try {
                        unserialize(s, obj.*(attr.second));
                    }
                    catch (const std::exception & e) {
                        std::cerr << e.what() << std::endl;
                    }
                });
            };

            template<typename T>
            static void serialize(std::ostream &, std::string_view, const std::unique_ptr<T> &) {}

            template<typename T>
            static void unserialize(std::istream &, std::unique_ptr<T> &) {}

            template<typename T>
            static void serialize(std::ostream &, std::string_view, const std::shared_ptr<T> &) {}

            template<typename T>
            static void unserialize(std::istream &, std::shared_ptr<T> &) {}

            template<typename T, typename = std::enable_if_t<!std::is_enum<T>::value>>
            static void serialize(std::ostream & s, std::string_view name, const T & attr) { s << attr << " "; }

            template<typename T, typename = std::enable_if_t<!std::is_enum<T>::value>>
            static void unserialize(std::istream & s, T & attr) { s >> attr; }

            template<typename T, typename = std::enable_if_t<std::is_enum<T>::value>>
            static void serialize(std::ostream & s, std::string_view name, T attr) { s << (int) attr << " "; };

            template<typename T, typename = std::enable_if_t<std::is_enum<T>::value>>
            static void unserialize(std::istream & s, const T & attr) { s >> (int &) attr; }
        };

        struct Binary {
            static void startSerialize(std::ostream & s) {}
            static void endSerialize(std::ostream & s) {}
            static void serializeNewField(std::ostream & s) {}

            template<typename T, typename Tuple>
            static void unserialize(std::istream & s, T & obj, const Tuple & tuple) {
                pmeta::tuple_for_each(tuple, [&s, &obj](const auto & attr) {
                    try {
                        unserialize(s, obj.*(attr.second));
                    }
                    catch (const std::exception & e) {
                        std::cerr << e.what() << std::endl;
                    }
                });
            };

            template<typename T>
            static void serialize(std::ostream &, std::string_view, const std::unique_ptr<T> &) {}

            template<typename T>
            static void unserialize(std::istream &, std::unique_ptr<T> &) {}

            template<typename T>
            static void serialize(std::ostream &, std::string_view, const std::shared_ptr<T> &) {}

            template<typename T>
            static void unserialize(std::istream &, std::shared_ptr<T> &) {}

            template<typename T, typename = std::enable_if_t<!std::is_enum<T>::value>>
            static void serialize(std::ostream & s, std::string_view name, const T & attr) { s.write(&attr, sizeof(attr)); }

            template<typename T, typename = std::enable_if_t<!std::is_enum<T>::value>>
            static void unserialize(std::istream & s, T & attr) { s.read(&attr, sizeof(attr)); }
        };

        struct None {
            static void startSerialize(std::ostream & s) {}
            static void endSerialize(std::ostream & s) {}
            static void serializeNewField(std::ostream & s) {}
            template<typename T, typename Tuple>
            static void unserialize(std::istream & s, T & obj, const Tuple & tuple) {}
            template<typename Auto>
            static void serialize(std::ostream &, std::string_view, const Auto &) {}
            template<typename Auto>
            static void unserialize(std::istream &, Auto &) {}
        };
    }
}
