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

#include "read_file.hpp"


#ifdef __linux__
#include <fcntl.h>
#endif

#include <boost/log/trivial.hpp>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <iostream>

#include <fmt/format.h>

namespace fesutils {

    read_file_source::read_file_source(const std::string& filepath_, std::size_t block_size_) :
    filepath(filepath_),
    block_size(block_size_) {




        std::vector<uint8_t> buffer(100);

        fd = std::fopen(filepath.c_str(),"rb");

        if(fd == nullptr)
            throw std::runtime_error("read_file_source: cannot open file with path: " + this->filepath);

        size_t byte_read = std::fread(buffer.data(),1, buffer.size(), fd);

        size_t idx = 0;
        size_t total_offset = 0;
        if(buffer[idx] == '#') { // Header must be skipped

            // Load more data and store offset, until we find '\n' not followed by '#'
            while(true) {
                idx++;
                if(idx >= byte_read)
                {
                    total_offset += byte_read;
                    byte_read = std::fread(buffer.data(),1, buffer.size(),fd);
                    if (byte_read == 0 || byte_read == 1) {
                        // EOF: no data left to read, and we are not even finished with the header
                        BOOST_LOG_TRIVIAL(error) << "No CV data in file " << this->filepath;
                        throw std::runtime_error("No data in CV file");
                    }
                    idx = 1;
                }
                if(buffer[idx-1] == '\n' && buffer[idx] != '#') {
                    // First actual data starts at idx (+ previous offset)
                    break;
                }
            }
            total_offset += idx;
        }

        // Reset seek to the start of the actual data
        std::fseek(fd, total_offset, SEEK_SET);

#ifdef __linux__
        int error = posix_fadvise(fileno(fd), total_offset,0, POSIX_FADV_SEQUENTIAL);
        if(error != 0) {BOOST_LOG_TRIVIAL(info) << "Non-zero fadvise return code " << error << " for POSIX_FADV_SEQUENTIAL, possible performance hit.";}
        error = posix_fadvise(fileno(fd), total_offset,0, POSIX_FADV_NOREUSE);
        if(error != 0) {BOOST_LOG_TRIVIAL(info) << "Non-zero fadvise return code " << error << " for POSIX_FADV_NOREUSE, possible performance hit.";}
        error = posix_fadvise(fileno(fd), total_offset,0, POSIX_FADV_WILLNEED);
        if(error != 0) {BOOST_LOG_TRIVIAL(info) << "Non-zero fadvise return code " << error << " for POSIX_FADV_WILLNEED, possible performance hit.";}
#endif


    }

    bool read_file_source::operator()(std::shared_ptr<std::vector<uint8_t>>& block) {
        block =  std::make_shared<std::vector<uint8_t>>(this->block_size);

        std::size_t size_loaded = std::fread(block->data(), 1, block->size(), this->fd);

        if(size_loaded == 0) {
            return false; // EOF
        }

        if(size_loaded < this->block_size) {
            // resize vector to actual size
            block->resize(size_loaded);
        }
        return true;
    }

    read_space_separated_file_source::read_space_separated_file_source(const std::string& filepath_,
                                                                       std::size_t block_size_)
       : read_file_source(filepath_, block_size_),
         processed_count(0),
         error_count(0) {

    }

    void find_eol_indexes(std::vector<size_t>& eol_index, const std::vector<uint8_t>& buffer) {
        for(size_t i = 0; i < buffer.size(); i++) {
            if(buffer[i] == '\n')
            {
                eol_index.push_back(i);
            }
        }
    }

    void line_from_eol_indexes(std::shared_ptr<std::vector<std::string>> lines_sptr,
                                std::string& remainder,
                                const std::vector<uint8_t>& buffer,
                                const std::vector<size_t>& eol_index,
                               size_t& processed_count, size_t& error_count) {
        size_t size_eol_index = eol_index.size();
        if(eol_index.size() == 0) {
            // Special case: no newline in block
            std::string s((char*) (buffer.data()), buffer.size() );
            remainder += s;
            return;
        }
        size_t last_idx = 0;
        bool offset_one_char = false;
        for(int i = 0; i < eol_index.size(); i++) {
            const size_t idx = eol_index[i];
            processed_count++;
            if(idx == 0) {
                // Special case where the buffer starts with \n
                offset_one_char = false;
                last_idx = 1;
                lines_sptr->emplace_back("");
                continue;
            }
            const size_t additional_offset = (offset_one_char) ? 1 : 0;
            offset_one_char = true;
            const size_t count = idx - last_idx;
            std::string s((char*) (buffer.data() + last_idx + additional_offset), count - additional_offset);
            last_idx = idx;


            if (s.find('#') != std::string::npos) { // Ignore comments in the body of the file (this can happen when resuming)
                error_count++;
                BOOST_LOG_TRIVIAL(warning) << "Unexpected comment line skipped: " << s << " (" << error_count << " encountered so far)";
                continue;
            }

            if(std::all_of(s.begin(), s.end(), [](char c){return std::isspace(static_cast<unsigned char>(c));})) { // Skip empty line
                continue;
            }

            lines_sptr->emplace_back(std::move(s));

        }
        // guarantee: eol_index.size() != 0    ==>  lines_v.size() >= 1
        lines_sptr->front().insert(0, remainder);
        const size_t count = (eol_index.back() == 1) ? 0: 1;
        if((eol_index.back() + 1) == buffer.size()) {
            // Last \n is at exact end of buffer
            remainder = "";
        }else {
            std::string s((char*) (buffer.data() + eol_index.back() + 1), buffer.size() - eol_index.back() -1 );
            remainder = std::move(s);
        }
    }

    bool read_space_separated_file_source::operator()(std::shared_ptr<std::vector<std::string>>& lines_sptr) {

        lines_sptr = std::make_shared<std::vector<std::string>>();

        std::shared_ptr<std::vector<uint8_t>> raw_block;

        // Read binary data from file
        bool ret = read_file_source::operator()(raw_block);

        if(!ret)
            return false;

        std::vector<size_t> eol_index;

        // Find where the \n are
        find_eol_indexes(eol_index, *raw_block);

        // Separate into line, and deal with the unfinished last line (this->remainder)
        line_from_eol_indexes(lines_sptr,this->remainder, *raw_block, eol_index, this->processed_count, this->error_count);


        return true;
    }
}