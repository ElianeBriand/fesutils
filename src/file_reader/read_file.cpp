//
// Created by eliane on 16/03/2020.
//

#include "read_file.hpp"


#ifdef __linux__
#include <fcntl.h>
#endif

#include <boost/log/trivial.hpp>


#include <cstdio>
#include <cstdlib>
#include <stdexcept>

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
       : read_file_source(filepath_, block_size_) {

    }

    bool read_space_separated_file_source::operator()(std::tuple<std::vector<std::string_view>,
                                                      std::shared_ptr<std::string>>& tokenized_block) {
        std::shared_ptr<std::vector<uint8_t>> raw_block;

        bool ret = read_file_source::operator()(raw_block);

        if(!ret)
            return false;



        return true;
    }
}