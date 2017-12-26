#pragma once

#include <iostream>
#include <unordered_map>
#include <tuple>
#include <string>
#include <sstream>
#include <fstream>
#include "meta/for_each.hpp"
#include "chop.hpp"

namespace putils {
    // Implementation details
    namespace {
        template<typename T, typename String>
        void extract(T && attr, String && val) {
            if constexpr (std::is_same<pmeta_typeof(attr), std::string>::value)
                attr = val;
            else {
                std::stringstream s(FWD(val));
                s >> attr;
            }
        };
    }

    namespace {
        template<typename ...Fields>
        void parseFieldsImplem(const std::tuple<Fields...> &,
                               const std::tuple<std::function<void(Fields)>...> &,
                               std::index_sequence<>) {}

        template<typename ...Fields, size_t I, size_t ...Is>
        void parseFieldsImplem(const std::tuple<Fields...> & fields,
                               const std::tuple<std::function<void(Fields)>...> & actions,
                               std::index_sequence<I, Is...>) {
            std::get<I>(actions)(std::get<I>(fields));
            parseFieldsImplem(fields, actions, std::index_sequence<Is...>());
        };

        template<typename ...Fields>
        void parseFields(const std::tuple<Fields...> & fields,
                         const std::tuple<std::function<void(Fields)>...> & actions) {
            parseFieldsImplem(fields, actions, std::index_sequence_for<Fields...>());
        }
    }

    //
    // CSV Lexer
    // Template arguments are:
    //      the separator used in the CSV file
    //      the "key" type (first element of each row)
    //      the list of types following the key on each row
    //
    // For instance, for a file like the following:
    //      "1;this is a test;42.0"
    // You want to declare putils::Csv<';', int, std::string, double>
    //
    template<char Separator, typename Key, typename ...Fields>
    class Csv {
        // Constructors
    public:
        // Initialize from stream
        Csv(std::istream & s, bool ignoreFirstLine = false) {
            fillRows(s, ignoreFirstLine);
        }

        // Initialize from file
        Csv(std::string_view fileName, bool ignoreFirstLine = false) {
            std::ifstream s(std::string(fileName));
            fillRows(s, ignoreFirstLine);
        }

        Csv() {}

    public:
        void dump(std::ostream & s) const {
            for (auto & p : _orderedRows) {
                s << p->first;
                pmeta::tuple_for_each(p->second, [&s](auto & attr) {
                    s << Separator << attr;
                });
                s << std::endl;
            }
        }

        // Get the row for specified key
    public:
        const std::tuple<Fields...> & get(const Key & key) const { return _rows.at(key); }

        // Get all rows
    public:
        const std::vector<std::pair<const Key, std::tuple<Fields...>> *> & getRows() const { return _orderedRows; }

    public:
        void add(const Key & key, const std::tuple<Fields...> & t) {
            const auto & p = _rows.emplace(key, t);
            _orderedRows.emplace_back(&(*p.first));
        }

        void add(const Key & key, const Fields & ...fields) { add(key, std::make_tuple(fields...)); }

    public:
        void addRow(std::string line) {
            const auto pos = line.find(Separator);
            std::string skey = line.substr(0, pos);
            Key key;
            extract(key, skey);

            line = line.substr(pos + 1);

            std::tuple<Fields...> row;
            pmeta::tuple_for_each(row, [&line](auto & attr) {
                const auto pos = line.find(Separator);
                std::string val = line.substr(0, pos);
                line = line.substr(pos + 1);

                extract(attr, val);
            });

            const auto & p = _rows.emplace(std::move(key), std::move(row));
            _orderedRows.emplace_back(&(*p.first));
        }

    public:
        void parse(
                const std::function<void(Key)> & init,
                const std::tuple<std::function<void(Fields)>...> & actions,
                const std::function<void()> & end) const {
            for (const auto & p : _orderedRows) {
                init(p->first);
                parseFields(p->second, actions);
                end();
            }
        }

        // Constructor implementation detail
    private:
        void fillRows(std::istream & s, bool ignoreFirstLine) {
            if (ignoreFirstLine) {
                std::string dummy;
                std::getline(s, dummy);
            }

            for (std::string line; std::getline(s, line);)
                addRow(putils::chop(line));
        }

    private:
        std::unordered_map<Key, std::tuple<Fields...>> _rows;
        std::vector<std::pair<const Key, std::tuple<Fields...>> *> _orderedRows;
    };
}
