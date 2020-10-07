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

#include "ingest_grid_data.hpp"

#include <boost/log/trivial.hpp>

#include <boost/filesystem.hpp>
namespace bf = boost::filesystem;


#include <tbb/flow_graph.h>

#include "../../export/array_bindump.hpp"
#include "../read_file.hpp"
#include "../parse_space_separated_values_node.hpp"
#include "../../common/limiter_decrementer_node.hpp"
#include "grid_filler_node.hpp"
#include "../../progress/progress_printer.hpp"
#include "../../progress/progress_printer_nodes.hpp"
#include "../../common/NotImplementedError.hpp"


namespace fesutils {


    std::shared_ptr<GridData>
    ingest_grid_data(const GeneralOptions& options,
                     const bf::path& grid_path,
                     const PlumedDatHeader& header_grid,
                     const size_t& value_index,
                     const std::vector<size_t>& variable_index) {

        const unsigned int num_axis = variable_index.size();

        std::vector<unsigned int> bins_per_axis;
        std::vector<double> min_bin_value_per_axis;
        std::vector<double> max_bin_value_per_axis;
        for(const size_t& cv_field_idx: variable_index) {
            const double min = std::get<double>(header_grid.fields[cv_field_idx].attributes.at("min"));
            const double max = std::get<double>(header_grid.fields[cv_field_idx].attributes.at("max"));
            const double nbins = std::get<int>(header_grid.fields[cv_field_idx].attributes.at("nbins"));

            min_bin_value_per_axis.push_back(min);
            max_bin_value_per_axis.push_back(max);
            bins_per_axis.push_back(nbins);
        }


        std::shared_ptr<GridData> grid = GridData_factory(GridData_storage_class::inmemory,
                                                          variable_index.size(),
                                                          bins_per_axis,
                                                           min_bin_value_per_axis,
                                                           max_bin_value_per_axis,
                                                           true);




        std::optional<bf::path> has_valid_bindump = corresponding_bindump_exists(grid_path);

        if(has_valid_bindump.has_value()) {
            BOOST_LOG_TRIVIAL(info) << "Fast loading from bindump (" << has_valid_bindump.value().generic_string() << ")";

            bool retvalue = ingest_grid_data_from_bindump(options,
                                                 grid_path,
                                                 grid,
                                                 header_grid,
                                                 num_axis,
                                                 value_index,
                                                 variable_index,
                                                 has_valid_bindump.value());
            // LCOV_EXCL_START
            // Reason for coverage exclusion: defensive check, but will throw if error so unreachable
            if(!retvalue) {
                return nullptr;
            }
            // LCOV_EXCL_STOP

        } else {
            BOOST_LOG_TRIVIAL(info) << "Loading from text file...";

            bool retvalue = ingest_grid_data_from_originalfile(options,
                                                      grid_path,
                                                      grid,
                                                      header_grid,
                                                      num_axis,
                                                      value_index,
                                                      variable_index);

            if(!retvalue) {
                // LCOV_EXCL_START
                // Reason for coverage exclusion: defensive check, but will throw if error so unreachable
                return nullptr;
                // LCOV_EXCL_STOP
            }

        }



        return grid;
    }


    bool ingest_grid_data_from_bindump(const GeneralOptions& options,
                                       const bf::path& grid_file_path,
                                       std::shared_ptr<GridData> griddata_container,
                                       const PlumedDatHeader& header_grid,
                                       unsigned int num_axis,
                                       const size_t& value_index,
                                       const std::vector<size_t>& variable_index,
                                       const bf::path& bindump_path) {
        BOOST_LOG_TRIVIAL(debug) << "[NOTIMPLEMENTED] ingest_cv_data_from_bindump is not implemented";
        throw not_implemented_error("Not implemented");
        return false;
    }

    bool ingest_grid_data_from_originalfile(const GeneralOptions& options,
                                            const bf::path& grid_file_path,
                                            std::shared_ptr<GridData> griddata_container,
                                            const PlumedDatHeader& header_grid,
                                            unsigned int num_axis,
                                            const size_t& value_index,
                                            const std::vector<size_t>& variable_index) {

        tbb::flow::graph g_griddata;
        ProgressPrinter progress_printer;


        // Previsional count of line to process
        // (Will be off by a few, but that's cosmetic as it affects only the progress bar)
        progress_printer.set_todo_count(griddata_container->get_num_voxels());

        tbb::flow::source_node<std::shared_ptr<std::vector<std::string>>> read_file_grid(g_griddata,
                                                                                         read_space_separated_file_source(
                                                                                                 grid_file_path.generic_string(),
                                                                                                 1024*1024),
                                                                                         false);

        tbb::flow::limiter_node<std::shared_ptr<std::vector<std::string>>> limiter_input(g_griddata, 200);

        tbb::flow::function_node<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::vector<std::string>>, tbb::flow::queueing> progress_entry_gate(
                g_griddata, 1, PerLine_TodoToInflight_node(progress_printer));



        tbb::flow::buffer_node<std::shared_ptr<std::vector<std::string>>> line_buffer(g_griddata);

        tbb::flow::function_node<std::shared_ptr<std::vector<std::string>>,
                std::shared_ptr<Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>, tbb::flow::queueing>
                double_parser_grid(g_griddata, 1, parse_space_separated_double_node(header_grid.fields.size()));



        tbb::flow::function_node<std::shared_ptr<Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>,
                unsigned int, tbb::flow::queueing> grid_filler(g_griddata, 1,
                                                        grid_filler_node(griddata_container, num_axis, value_index, variable_index));


        tbb::flow::function_node<unsigned int, tbb::flow::continue_msg, tbb::flow::queueing> progress_exit_gate(g_griddata,
                                                                                                            1,
                                                                                           PerNumItem_InflightToDone_node(
                                                                                                                           progress_printer));


        tbb::flow::function_node<tbb::flow::continue_msg,
                tbb::flow::continue_msg, tbb::flow::queueing> input_limiter_decrementer(g_griddata, 1,
                                                                   limiter_decrementer_node<tbb::flow::continue_msg>());


        tbb::flow::make_edge(read_file_grid, limiter_input);
        tbb::flow::make_edge(limiter_input, progress_entry_gate);
        tbb::flow::make_edge(progress_entry_gate, line_buffer);
        tbb::flow::make_edge(line_buffer, double_parser_grid);
        tbb::flow::make_edge(double_parser_grid, grid_filler);
        tbb::flow::make_edge(grid_filler, progress_exit_gate);
        tbb::flow::make_edge(progress_exit_gate, input_limiter_decrementer);
        tbb::flow::make_edge(input_limiter_decrementer, limiter_input.decrement);


        read_file_grid.activate();
        g_griddata.wait_for_all();

        progress_printer.finish();

        // Check that the grid file loaded only one or no value into each grid voxel
        // If not, then there was overwriting, this most probably indicates a problem
        // Or at least that the process could not be parallelized safely
        // (Post hoc check as normally grid file do abide by this)

        bool correct_write_access_pattern = griddata_container->check_no_or_one_write_access_everywhere();

        if(!correct_write_access_pattern) {
            BOOST_LOG_TRIVIAL(error) << "The grid file contain lines that resolve to the same voxel in the Grid";
            BOOST_LOG_TRIVIAL(error) << "This is not supposed to happen, and might be a bug in this program";
            BOOST_LOG_TRIVIAL(error) << "or that the grid file provided contain multiple grid";
            BOOST_LOG_TRIVIAL(error) << "(For fast parallel loading to be correct, each voxel must be set 0 or 1 time only.)";
            throw std::runtime_error("Grid file contains multiple value for single voxel");
        }


        return true;
    }

}