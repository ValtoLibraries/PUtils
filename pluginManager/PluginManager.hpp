#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <fstream>
#include <regex>
#include <iostream>
#include "Library.hpp"
#include "Directory.hpp"
#include "LibraryFactory.hpp"

namespace putils {
    //
    // PluginManager which either loads all libraries in a folder
    // or loads libraries contained in a CSV config file
    // Offers functions to execute a given function in all libraries,
    // in all libraries of certain categories, with or without
    // return values
    //
    class PluginManager {
        // Constructor
    public:
        // Loads any .dll or .so file in the directory specified by path
        // Unix libraries should be .so files and Windows libraries should be .dll files
        template<typename String>
        PluginManager(String && path) noexcept {
#ifdef _WIN32
            static std::regex			freg("^.*\\.dll$");
#else
            static std::regex freg("^.*\\.so");
#endif

            putils::Directory dir(FWD(path));

            dir.for_each([this](const putils::ADirectory::File & f) {
                if (std::regex_match(f.fullPath, freg)) {
                    try {
                        auto plugin = putils::LibraryFactory::make(f.fullPath);
                        _libraries.push_back(plugin);
                    }
                    catch (std::runtime_error & e) {
                        std::cerr << e.what() << std::endl;
                    }
                }
            });
        }

    public:
        // In each plugin, execute the [name] function, taking P as parameter
        template<typename ...P>
        void execute(std::string_view name, P && ...params) noexcept {
            for (auto plugin : _libraries)
                plugin->execute<void>(name, std::forward<P>(params)...);
        }

        // In each plugin, execute the [name] function, returning a T and taking a P as parameter
        // Returns a vector of all values returned
        template<typename T, typename ...P>
        std::vector<T> executeWithReturn(const std::string & name, P && ...params) noexcept {
            std::vector<T> ret;

            for (auto plugin : _libraries) {
                auto func = plugin->loadMethod<T, P...>(name);
                if (func != nullptr)
                    ret.push_back(func(std::forward<P>(params)...));
            }

            return ret;
        }

        // Attributes
    private:
        std::vector<putils::Library *> _libraries;

        // Coplien
    public:
        PluginManager(const PluginManager &) = delete;
        PluginManager & operator=(const PluginManager &) = delete;
    };
}
