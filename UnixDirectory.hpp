#pragma once

#ifdef __unix__

#include <sys/stat.h>
#include <dirent.h>
#include "ADirectory.hpp"
#include "concat.hpp"

namespace putils {
    class UnixDirectory final : public ADirectory {
    public:
        UnixDirectory(const std::string & path)
                :
                _handle(opendir(path.c_str())),
                _path(path),
                _goOn(true) {
            if (!_handle)
                throw std::runtime_error(putils::concat("No such directory: ", path));

            if (_path[_path.length() - 1] != '/')
                _path = _path + "/";
        }

        virtual ~UnixDirectory() noexcept {
            if (_handle != nullptr)
                closedir(_handle);
        }

    public:
        std::unique_ptr<File> getNextFile() noexcept override {
            if (_goOn == false || _handle == nullptr)
                return nullptr;

            struct dirent * ent = readdir(_handle);

            if (ent == nullptr) {
                _goOn = false;
                return nullptr;
            }


            std::string file(ent->d_name);

            if (file == "." || file == ".." || file.length() == 0)
                return getNextFile();

            auto fullPath = _path + file;
            struct stat sb;

            if (lstat(fullPath.c_str(), &sb) == -1) {
                _goOn = false;
                return nullptr;
            }

            return std::make_unique<File>(File{
                    file, fullPath,
                    S_ISDIR(sb.st_mode), S_ISLNK(sb.st_mode)
            });
        }

    private:
        DIR * _handle;
        std::string _path;
        bool _goOn;
    };
}

#endif