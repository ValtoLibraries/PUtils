#pragma once

#include <regex>

#include "WindowsLibrary.hpp"
#include "UnixLibrary.hpp"

namespace putils
{
    namespace
    {
        template<typename Str1, typename Str2>
        std::string addLibToPath(Str1 &&name, Str2 &&extension) noexcept
        {
            static std::regex end(std::string("^.*\\") + extension + "$");

            if (!std::regex_match(name, end))
            {
                std::string path = "";
                std::string lib = name;

                auto it = name.find_last_of("/");
                if (it != std::string::npos)
                {
                    path = name.substr(0, it + 1);
                    lib = name.substr(it + 1);
                }
                return path + "lib" + lib + extension;
            }
            else
                return name;
        }
    }

    namespace LibraryFactory
    {
        inline putils::Library *make(const std::string & name)
        {
            static std::unordered_map<std::string, std::unique_ptr<Library>> _register;

            if (_register.find(name) != _register.end())
                return _register.at(name).get();

#ifdef MINGW
            auto lib = std::make_unique<WindowsLibrary>(addLibToPath(name, ".dll"));
            _register.emplace(name, std::move(lib));
#elif defined(_WIN32)
            static std::regex		end(R"(^.*\.dll$)");

            const auto toLoad = std::regex_match(name, end) ? name : name + ".dll";
            auto lib = std::make_unique<WindowsLibrary>(toLoad);
            _register.emplace(name, std::move(lib));
#elif defined(__unix__)
            auto lib = std::make_unique<UnixLibrary>(addLibToPath(name, ".so"));
            _register.emplace(name, std::move(lib));
#endif

            return _register.at(name).get();
        }
    }
}
