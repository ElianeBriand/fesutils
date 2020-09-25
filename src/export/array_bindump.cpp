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
#include <fstream>
#include <stdint.h>

#include "array_bindump.hpp"
#include "../frontend_common.h"
#include "../common/fs_utils.hpp"


namespace {
    void reject_bindump(const bf::path& path, const std::string& reason) {
        BOOST_LOG_TRIVIAL(info) << "File " << path.generic_string() << " is not a valid bindump file.";
        BOOST_LOG_TRIVIAL(info) << "Reason: " << reason;
    }
}

namespace fesutils {

    std::optional<bf::path> corresponding_bindump_exists(const bf::path& path) {
        std::optional<bf::path> found_file; // Empty = not found

        BOOST_LOG_TRIVIAL(info) << "Searching for previously-generated bindump for file "<< path;

        bf::path potential_bindump_path = path;
        std::string potential_bindump_filename = path.leaf().generic_string() + ".bindump";
        potential_bindump_path.remove_leaf() /= potential_bindump_filename;

        bool fs_result = path_exists_isfile(potential_bindump_path);

        if(!fs_result) {
            reject_bindump(potential_bindump_path, path_notfile_whatistheproblem(potential_bindump_path));
            return found_file;
        }

        std::ifstream infile(potential_bindump_path.generic_string(), std::ios::in | std::ios::binary);

        if(infile.is_open()) {
            uint32_t magic_number_binheader;
            static_assert(sizeof(char) == sizeof(uint8_t));
            infile.read(reinterpret_cast<char*>(&magic_number_binheader), 4);



            if(magic_number_binheader != 0xFE5D0101) {
                reject_bindump(potential_bindump_path, "does not appear to be a fesutil bindump (missing magic number 0xFE5D1010)");
                return found_file;
            }

            uint32_t data_begin_offset_binheader; // Byte offset at which the actual data starts
            infile.read(reinterpret_cast<char*>(&data_begin_offset_binheader), 4);

            uint32_t format_version_binheader; // Format version
            infile.read(reinterpret_cast<char*>(&format_version_binheader), 4);

            if(format_version_binheader != 0x00000001) {
                std::string reason = std::string("wrong bindump version (got ") +
                        std::to_string(format_version_binheader) + ", expected 1)";
                reject_bindump(potential_bindump_path, reason);
                return found_file;
            }


            uint32_t size_original_file_binheader; // Byte offset at which the actual data starts
            infile.read(reinterpret_cast<char*>(&size_original_file_binheader), 4);

            BOOST_LOG_TRIVIAL(debug) << "Value of size_original_file_binheader = " << size_original_file_binheader;


            auto original_file_size =  bf::file_size(path);
            if(original_file_size != size_original_file_binheader) {
                BOOST_LOG_TRIVIAL(debug) << "Value of original_file_size = " << original_file_size;

                reject_bindump(potential_bindump_path, "size of original file now does not match the size in the header");
                BOOST_LOG_TRIVIAL(info) << "It is likely the file has changed since when the bindump was created";
                return found_file;
            }

        } else {
            // LCOV_EXCL_START
            // Reason for coverage exclusion: difficult to mock
            BOOST_LOG_TRIVIAL(info) << "File " << potential_bindump_path.generic_string() << " cannot be used as bindump";
            BOOST_LOG_TRIVIAL(info) << "Reason: file cannot be opened";
            return found_file;
            // LCOV_EXCL_STOP
        }

        found_file = potential_bindump_path;
        BOOST_LOG_TRIVIAL(info) << "Found usable bindump in file "<< potential_bindump_path.generic_string();
        return found_file;
    }
}