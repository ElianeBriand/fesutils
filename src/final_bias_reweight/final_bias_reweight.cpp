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

#include "final_bias_reweight.hpp"

#include <boost/log/trivial.hpp>

#include <tbb/flow_graph.h>


#include "../common/extract_from_string.hpp"
#include "../file_reader/read_file_header.hpp"
#include "../file_reader/read_file.hpp"
#include "../common/graph_node_type.hpp"




namespace fesutils {

    void do_final_bias_reweight(GeneralOptions& options,
                                const std::string& cvfile_path,
                                const std::string& metadgrid_path,
                                const std::string& ranges_min_rawstr,
                                const std::string& ranges_max_rawstr) {

        std::vector<float> range_min = extract_range_from_string(ranges_min_rawstr);
        std::vector<float> range_max = extract_range_from_string(ranges_max_rawstr);

        PlumedDatHeader header =  read_cv_file_header(cvfile_path);


        BOOST_LOG_TRIVIAL(info) << "Header information:";
        BOOST_LOG_TRIVIAL(info) << header;


        tbb::flow::graph g;

        tbb::flow::source_node<RAW_FILE_READ_BLOCK_TYPE> read_file_node(g,
                read_file_source(cvfile_path, options.fileIOPerfOptions.binaryFileReadBufferSize ));




    }

}