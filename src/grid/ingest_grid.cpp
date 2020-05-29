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

#include "ingest_grid.hpp"


#include <boost/filesystem.hpp>

#include <boost/log/trivial.hpp>

#include <tbb/flow_graph.h>

#include <Eigen/Dense>


#include "../file_reader/field_function_heuristics.hpp"
#include "../progress/progress_printer.hpp"
#include "../progress/progress_printer_nodes.hpp"
#include "../file_reader/read_file.hpp"
#include "../file_reader/parse_space_separated_values_node.hpp"
#include "grid_filler_node.hpp"
#include "../common/limiter_decrementer_node.hpp"


namespace fesutils {

    std::shared_ptr<Grid> ingest_fes_grid(const GeneralOptions& options,
                                            const std::string& grid_path,
                                            PlumedDatHeader& header_grid,
                                            const std::optional<std::string>& override_freenergy_field_name) {
        BOOST_LOG_TRIVIAL(info) << "Ingesting FES data from grid file: " << grid_path;

        std::shared_ptr<Grid> metad_grid = std::make_shared<Grid>(header_grid);


        size_t num_fields_grid = header_grid.fields.size();

        size_t bias_idx = 0;
        if(override_freenergy_field_name.has_value()) {
            bias_idx = header_grid.find_field_index_from_name(override_freenergy_field_name.value());
        } else {
            std::optional<size_t> maybe_bias_indx =  find_likely_fenerg_field_index(header_grid);
            if(!maybe_bias_indx.has_value()) {
                BOOST_LOG_TRIVIAL(error) << "Could not identify free energy field. Please specify using argument --fefield";
                throw std::runtime_error("Could not identify free energy field.");
            }
            bias_idx = maybe_bias_indx.value();
        }


        std::string binarydump_path = grid_path + ".bindump";

        BOOST_LOG_TRIVIAL(info) << "Checking for previously-generated grid binary dump file: " << binarydump_path;

        bool did_load = false;
        if(boost::filesystem::exists(binarydump_path)) {
            BOOST_LOG_TRIVIAL(info) << "Found. Loading...";
            bool did_load = metad_grid->load_binrepr_from_file(binarydump_path);
            if(did_load) {
                BOOST_LOG_TRIVIAL(info) << "Successfully loaded binary dump file.";
                BOOST_LOG_TRIVIAL(info) << "NB: delete/move this file to re-load from original data. (eg: if the file has changed)";
            } else {
                // LCOV_EXCL_START
                // Reason for coverage exclusion: Will not unit test FS failure or file corruption for this routine
                BOOST_LOG_TRIVIAL(info) << "Could not load from binary dump. Regenerating from original file.";
                // LCOV_EXCL_STOP
            }
        } else {
            BOOST_LOG_TRIVIAL(info) << "Not found. Generating from original file.";
        }

        if(!did_load) {
            Progress_printer progress_printer;
            tbb::flow::graph g_grid;

            tbb::flow::source_node<std::shared_ptr<std::vector<std::string>>> read_file_grid(g_grid,
                                                                                             read_space_separated_file_source(
                                                                                                     grid_path,
                                                                                                     options.fileIOPerfOptions.binaryFileReadBufferSize),
                                                                                             false);

            tbb::flow::limiter_node<std::shared_ptr<std::vector<std::string>>> limiter_input(g_grid, 100);

            tbb::flow::buffer_node<std::shared_ptr<std::vector<std::string>>> line_buffer(g_grid);

            tbb::flow::function_node<std::shared_ptr<std::vector<std::string>>,
            std::shared_ptr<Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>, tbb::flow::rejecting>
                    double_parser_grid(g_grid, 2, parse_space_separated_double_node(num_fields_grid));


            tbb::flow::function_node<std::shared_ptr<Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>,
            std::shared_ptr<Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>> workpacket_registerer(
                    g_grid, 1,
                    progress_work_packet_registerer<std::shared_ptr<Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>>(
                            progress_printer));

            tbb::flow::function_node<std::shared_ptr<Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>,
            tbb::flow::continue_msg> bias_grid_fill(g_grid, 1,
                                                    grid_filler_node(metad_grid, bias_idx));

            tbb::flow::function_node<tbb::flow::continue_msg, tbb::flow::continue_msg> workpacket_done_reporter(g_grid,
                                                                                                                1,
                                                                                                                progress_work_packet_done_reporter<tbb::flow::continue_msg>(
                                                                                                                        progress_printer));

            tbb::flow::function_node<tbb::flow::continue_msg,
            tbb::flow::continue_msg> input_limiter_decrementer(g_grid, 1,
                                                               limiter_decrementer_node<tbb::flow::continue_msg>());


            tbb::flow::make_edge(read_file_grid, limiter_input);
            tbb::flow::make_edge(limiter_input, line_buffer);
            tbb::flow::make_edge(line_buffer, double_parser_grid);
            tbb::flow::make_edge(double_parser_grid, workpacket_registerer);
            tbb::flow::make_edge(workpacket_registerer, bias_grid_fill);
            tbb::flow::make_edge(bias_grid_fill, workpacket_done_reporter);
            tbb::flow::make_edge(workpacket_done_reporter, input_limiter_decrementer);
            tbb::flow::make_edge(input_limiter_decrementer, limiter_input.decrement);


            read_file_grid.activate();

            g_grid.wait_for_all();
            progress_printer.finish();

            BOOST_LOG_TRIVIAL(info) << "";
            BOOST_LOG_TRIVIAL(info) << "Finished loading free energy grid.";

            BOOST_LOG_TRIVIAL(info) << "Preparing to dump grid to binary file: " << binarydump_path;
            bool did_dump = metad_grid->dump_binrepr_to_file(binarydump_path);
            if(did_dump) {
                BOOST_LOG_TRIVIAL(info) << "Successfully dumped grid. Next loading will be faster.";
            }else {
                // LCOV_EXCL_START
                // Reason for coverage exclusion: Will not unit test FS failure or permission error for this routine
                BOOST_LOG_TRIVIAL(info)
                    << "Unsuccessful in dumping the grid. Check write permission. This is not a fatal error.";
                // LCOV_EXCL_STOP
            }


        }


        return metad_grid;
    }
}