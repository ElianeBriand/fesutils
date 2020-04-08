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

#include <Eigen/Dense>
#include <iostream>

#include "../common/extract_from_string.hpp"
#include "../file_reader/read_file_header.hpp"
#include "../file_reader/read_file.hpp"
#include "../common/graph_node_type.hpp"
#include "../common/print_lines_node.hpp"
#include "../common/dummy_sink_node.hpp"
#include "../file_reader/parse_space_separated_values_node.hpp"
#include "../grid/Grid.hpp"
#include "../grid/grid_utils.hpp"
#include "../common/limiter_decrementer_node.hpp"


namespace fesutils {

    // LCOV_EXCL_START
    // Reason for coverage exclusion: subject to system testing, not unit testing
    // System test script: st_final_bias_reweight

    void do_final_bias_reweight(GeneralOptions& options,
                                const final_bias_reweight_args& args) {

        std::vector<float> range_min = extract_range_from_string(args.ranges_min_rawstr);
        std::vector<float> range_max = extract_range_from_string(args.ranges_max_rawstr);

        // ///// First read the bias grid file
        PlumedDatHeader header_grid =  read_cv_file_header(args.metadgrid_path);

        BOOST_LOG_TRIVIAL(info) << "Bias grid file: " << args.metadgrid_path;
        BOOST_LOG_TRIVIAL(info) << "\n";
        BOOST_LOG_TRIVIAL(info) << "-- BEGIN HEADER --";
        BOOST_LOG_TRIVIAL(info) << header_grid;
        BOOST_LOG_TRIVIAL(info) << "-- END   HEADER --";
        BOOST_LOG_TRIVIAL(info) << "";

        size_t num_fields_grid = header_grid.fields.size();

        size_t bias_idx = 0;
        if(args.biasfield.has_value()) {
            auto it = std::find_if(header_grid.fields.begin(), header_grid.fields.end(), [&](const Field& f) {
                return f.name == args.biasfield.value(); // Find field that has bias
            });
            bias_idx = std::distance(header_grid.fields.begin(), it);
        } else {
            std::optional<size_t> maybe_bias_indx =  find_likely_bias_field_index(header_grid);
            if(!maybe_bias_indx.has_value()) {
                BOOST_LOG_TRIVIAL(error) << "Could not identify bias field. Please specify using argument --biasfield";
                std::exit(2);
            }
            bias_idx = maybe_bias_indx.value();
        }


        BOOST_LOG_TRIVIAL(info) << "Field identified to be the bias value: " << header_grid.fields[bias_idx].name;

        Grid metad_grid(header_grid);
        BOOST_LOG_TRIVIAL(info) << "\n";
        BOOST_LOG_TRIVIAL(info) << "-- BEGIN GRID --";
        BOOST_LOG_TRIVIAL(info) << metad_grid;
        BOOST_LOG_TRIVIAL(info) << "-- END   GRID --";
        BOOST_LOG_TRIVIAL(info) << "";
        BOOST_LOG_TRIVIAL(info) << "Number of grid fields: " << num_fields_grid;
        BOOST_LOG_TRIVIAL(info) << "options.fileIOPerfOptions.binaryFileReadBufferSize: " << options.fileIOPerfOptions.binaryFileReadBufferSize;
        tbb::flow::graph g_grid;

        tbb::flow::source_node< std::shared_ptr<std::vector<std::string>> > read_file_grid(g_grid,
                                      read_space_separated_file_source(args.metadgrid_path,
                                                               options.fileIOPerfOptions.binaryFileReadBufferSize ),
                                                               false);

        tbb::flow::limiter_node< std::shared_ptr<std::vector<std::string>> > limiter_input( g_grid, 100 );

        tbb::flow::buffer_node< std::shared_ptr<std::vector<std::string>> > line_buffer(g_grid);

        tbb::flow::function_node< std::shared_ptr<std::vector<std::string>>,
                std::shared_ptr<Eigen::Array<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor>>, tbb::flow::rejecting >
                double_parser_grid(g_grid, 2, parse_space_separated_double_node(num_fields_grid));

        tbb::flow::function_node< std::shared_ptr<Eigen::Array<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor>>,
                                  tbb::flow::continue_msg > input_limiter_decrementer(g_grid, 1,
                          limiter_decrementer_node<std::shared_ptr<Eigen::Array<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor>>>());

        tbb::flow::make_edge( read_file_grid, limiter_input );
        tbb::flow::make_edge( limiter_input, line_buffer );
        tbb::flow::make_edge( line_buffer, double_parser_grid );
        tbb::flow::make_edge( double_parser_grid, input_limiter_decrementer );
        tbb::flow::make_edge( input_limiter_decrementer, limiter_input.decrement );

        read_file_grid.activate();

        g_grid.wait_for_all();

        std::cout << "\n" <<  std::endl;
        return;


        // ///// Then read the CV file and do the histogram


        PlumedDatHeader header_cv =  read_cv_file_header(args.cvfile_path);

        BOOST_LOG_TRIVIAL(info) << "CV file: " << args.cvfile_path;
        BOOST_LOG_TRIVIAL(info) << "Header information:";
        BOOST_LOG_TRIVIAL(info) << header_cv;

        size_t num_fields_cv = header_cv.fields.size();

        tbb::flow::graph g_cv;

        tbb::flow::source_node<std::shared_ptr<std::vector<std::string>>> read_file_cv(g_cv,
                                          read_space_separated_file_source(args.cvfile_path,
                                                  options.fileIOPerfOptions.binaryFileReadBufferSize ),
                                                  false);


        tbb::flow::function_node<std::shared_ptr<std::vector<std::string>>,
        std::shared_ptr<Eigen::Array<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor>>>
            double_parser_cv(g_cv, 1, parse_space_separated_double_node(num_fields_cv));
        tbb::flow::make_edge( read_file_cv, double_parser_cv );

        read_file_cv.activate();

        g_cv.wait_for_all();


    }

    // LCOV_EXCL_STOP

}