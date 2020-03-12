//
// Created by eliane on 20/03/2020.
//

#include "fs_utils.hpp"


#include <boost/filesystem.hpp>

namespace fesutils {

    bool path_exists_isfile(const std::string& filepath) {
        boost::filesystem::path p(filepath);

        if (exists(p) && is_regular_file(p))
            return true;
        else
            return false;
    }

    std::string path_notfile_whatistheproblem(const std::string& filepath) {
        boost::filesystem::path p(filepath);

        if(!exists(p)) {
            return "path does not exist";
        }
        if(is_directory(p)) {
            return "path is a directory, not a file";
        }
        if(!is_regular_file(p))  {
            return "file is not a regular file";
        }

        return "unknown";
    }

}