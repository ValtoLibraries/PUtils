#pragma once

#include <string>
#include <functional>
#include <memory>
#include <vector>

namespace putils {
    class ADirectory {
        // File structure
    public:
        struct File {
            std::string name;
            std::string fullPath;
            bool isDirectory;
            bool isLink;
        };

        // Destructor
        virtual ~ADirectory() noexcept = default;

        // Read the next entry in the directory
        virtual std::unique_ptr<File> getNextFile() noexcept = 0;

        // Apply a function to each file in the directory
        void for_each(const std::function<void(const File &)> & func) noexcept {
            std::unique_ptr < File > f;

            while ((f = getNextFile()) != nullptr)
                func(*f);
        }

        // Get a list of all the files
        std::vector<File> getFiles() noexcept {
            std::vector<File> ret;

            for_each([&ret](const File & f) {
                ret.push_back(std::move(f));
            });

            return ret;
        }
    };
}