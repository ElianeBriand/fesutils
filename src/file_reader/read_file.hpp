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

#ifndef FESUTILS_READ_FILE_HPP
#define FESUTILS_READ_FILE_HPP

#include <string>

#include <tbb/flow_graph.h>

namespace fesutils{



    class read_file_source {
    public:

        read_file_source(const std::string& filepath_, std::size_t block_size_);

        virtual bool operator()(std::shared_ptr<std::vector<uint8_t>>& block);

    protected:
        FILE *fd;

        std::string filepath;
        std::size_t block_size;


    };


    class read_space_separated_file_source: public read_file_source {
    public:
        read_space_separated_file_source(const std::string& filepath_, std::size_t block_size_);

        bool operator()(std::shared_ptr<std::vector<std::string>>& lines_sptr);

    protected:
        std::string remainder;

        size_t processed_count;
        size_t error_count;

    };

    // Internals exposed for testing

    void find_eol_indexes(std::vector<size_t>& eol_index,const std::vector<uint8_t>& buffer);
    void line_from_eol_indexes(std::shared_ptr<std::vector<std::string>> lines_sptr,
                               std::string& remainder,
                               const std::vector<uint8_t>& buffer,
                               const std::vector<size_t>& eol_index,
                               size_t& processed_count, size_t& error_count);

    }



#endif //FESUTILS_READ_FILE_HPP
