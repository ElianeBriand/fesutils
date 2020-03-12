//
// Created by eliane on 16/03/2020.
//

#ifndef FESUTILS_READ_FILE_HPP
#define FESUTILS_READ_FILE_HPP

#include <string>

#include <tbb/flow_graph.h>

namespace fesutils{



    class read_file_source {
    public:
        std::string filepath;
        std::size_t block_size;

        read_file_source(const std::string& filepath_, std::size_t block_size_);

        bool operator()(std::shared_ptr<std::vector<uint8_t>>& block);

    protected:
        FILE *fd;

    };


    class read_space_separated_file_source: public read_file_source {
    public:
        std::string filepath;
        std::size_t block_size;

        read_space_separated_file_source(const std::string& filepath_, std::size_t block_size_);

        bool operator()(std::tuple<std::vector<std::string_view>, std::shared_ptr<std::string>>& tokenized_block);

    private:
        FILE *fd;

    };

}



#endif //FESUTILS_READ_FILE_HPP
