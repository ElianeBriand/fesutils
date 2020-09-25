/*
 * Copyright (c) 2020 Eliane Briand
 *
 * This file is part of fesutils.
 *
 * fesutils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * fesutils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with fesutils.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#include "fs_utils.hpp"


#include <boost/filesystem.hpp>

namespace fesutils {

    bool path_exists_isfile(const std::string& filepath) {
        boost::filesystem::path p(filepath);
        return fesutils::path_exists_isfile(p);
    }

    std::string path_notfile_whatistheproblem(const std::string& filepath) {
        boost::filesystem::path p(filepath);
        return fesutils::path_notfile_whatistheproblem(p);
    }

    bool path_exists_isfile(const bf::path& filepath) {
        if (exists(filepath) && is_regular_file(filepath))
            return true;
        else
            return false;
    }

    std::string path_notfile_whatistheproblem(const bf::path& filepath) {

        if(!exists(filepath)) {
            return "path does not exist";
        }

        // LCOV_EXCL_START
        // Reason for coverage exclusion: not worth time to mockup

        if(is_directory(filepath)) {
            return "path is a directory, not a file";
        }
        if(!is_regular_file(filepath))  {
            return "file is not a regular file";
        }

        return "unknown";
        // LCOV_EXCL_STOP
    }

}