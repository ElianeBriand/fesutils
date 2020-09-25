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

#include <boost/log/trivial.hpp>

#include <tbb/flow_graph.h>

#include "ingest_cv_data.hpp"
#include "../../export/array_bindump.hpp"
#include "../../progress/progress_printer.hpp"
#include "../read_file.hpp"
#include "../parse_space_separated_values_node.hpp"
#include "../../progress/progress_printer_nodes.hpp"
#include "CVData_filler_node.hpp"
#include "../../common/limiter_decrementer_node.hpp"

namespace fesutils {

    bool
    ingest_cv_data(const GeneralOptions& options,
                   const bf::path& cv_file_path,
                   std::shared_ptr<CVData> cv_data_container,
                   const PlumedDatHeader& cv_file_header,
                   const std::vector<std::string>& cv_varnames,
                   const std::string& metad_bias_varname,
                   const std::vector<std::string>& other_values_varnames) {

        size_t metad_bias_var_idx = cv_file_header.find_field_index_from_name(metad_bias_varname);

        std::vector<size_t> cv_vars_idx;
        for(const std::string& cv_varname: cv_varnames) {
            size_t curr_cv_var_idx = cv_file_header.find_field_index_from_name(cv_varname);
            cv_vars_idx.push_back(curr_cv_var_idx);
        }

        std::vector<size_t> other_vars_idx;
        for(const std::string& other_values_varname : other_values_varnames) {
            size_t curr_other_var_idx = cv_file_header.find_field_index_from_name(other_values_varname);
            other_vars_idx.push_back(curr_other_var_idx);
        }

        std::optional<bf::path> has_valid_bindump = corresponding_bindump_exists(cv_file_path);

        if(has_valid_bindump.has_value()) {
            BOOST_LOG_TRIVIAL(info) << "Fast loading from bindump (" << has_valid_bindump.value().generic_string() << ")";

            return ingest_cv_data_from_bindump(options,
                                               cv_file_path,
                                               cv_data_container,
                                               cv_file_header,
                                               cv_varnames,
                                               metad_bias_varname,
                                               other_values_varnames,
                                               cv_vars_idx,
                                               metad_bias_var_idx,
                                               other_vars_idx,
                                               has_valid_bindump.value());

        } else {
            BOOST_LOG_TRIVIAL(info) << "Loading from text file...";

            return ingest_cv_data_from_originalfile(options,
                                               cv_file_path,
                                               cv_data_container,
                                               cv_file_header,
                                               cv_varnames,
                                               metad_bias_varname,
                                               other_values_varnames,
                                               cv_vars_idx,
                                               metad_bias_var_idx,
                                               other_vars_idx);

        }
    }


    bool ingest_cv_data_from_bindump(const GeneralOptions& options,
                                     const bf::path& cv_file_path,
                                     std::shared_ptr<CVData> cv_data_container,
                                     const PlumedDatHeader& cv_file_header,
                                     const std::vector<std::string>& cv_varnames,
                                     const std::string& metad_bias_varname,
                                     const std::vector<std::string>& other_values_varnames,
                                     const std::vector<size_t>& cv_vars_idx,
                                     const size_t& metad_bias_var_idx,
                                     const std::vector<size_t>& other_vars_idx,
                                     const bf::path& bindump_path) {
        BOOST_LOG_TRIVIAL(debug) << "[NOTIMPLEMENTED] ingest_cv_data_from_bindump is not implemented";
        throw std::runtime_error("Not implemented");
        return false;
    }

    bool ingest_cv_data_from_originalfile(const GeneralOptions& options,
                                          const bf::path& cv_file_path,
                                          std::shared_ptr<CVData> cv_data_container,
                                          const PlumedDatHeader& cv_file_header,
                                          const std::vector<std::string>& cv_varnames,
                                          const std::string& metad_bias_varname,
                                          const std::vector<std::string>& other_values_varnames,
                                          const std::vector<size_t>& cv_vars_idx,
                                          const size_t& metad_bias_var_idx,
                                          const std::vector<size_t>& other_vars_idx) {


        //Progress_printer progress_printer;
        tbb::flow::graph g_cvdata;

        tbb::flow::source_node<std::shared_ptr<std::vector<std::string>>> read_file_node(g_cvdata,
                                                                                         read_space_separated_file_source(
                                                                                                 cv_file_path.generic_string(),
                                                                                                 options.fileIOPerfOptions.binaryFileReadBufferSize),
                                                                                         false);

        tbb::flow::limiter_node<std::shared_ptr<std::vector<std::string>>> limiter_input_node(g_cvdata, 300);

        tbb::flow::buffer_node<std::shared_ptr<std::vector<std::string>>> line_buffer_node(g_cvdata);

        tbb::flow::function_node<std::shared_ptr<std::vector<std::string>>,
        std::shared_ptr<Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>, tbb::flow::rejecting>
                double_parser_node(g_cvdata, 1, parse_space_separated_double_node(cv_file_header.fields.size()));

        /*
        tbb::flow::function_node<std::shared_ptr<Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>,
        std::shared_ptr<Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>> workpacket_registerer_node(
                g_cvdata, 1,
                progress_work_packet_registerer<std::shared_ptr<Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>>(
                        progress_printer));
        */

        tbb::flow::function_node<std::shared_ptr<Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>,
        tbb::flow::continue_msg> cv_fill_node(g_cvdata, 1,
                                                CVData_filler_node(cv_data_container, cv_vars_idx,metad_bias_var_idx, other_vars_idx));

        /*
        tbb::flow::function_node<tbb::flow::continue_msg, tbb::flow::continue_msg> workpacket_done_reporter_node(g_cvdata,
                                                                                                            1,
                                                                                                            progress_work_packet_done_reporter<tbb::flow::continue_msg>(
                                                                                                                           progress_printer));
        */

        tbb::flow::function_node<tbb::flow::continue_msg,
        tbb::flow::continue_msg> input_limiter_decrementer_node(g_cvdata, 1,
                                                           limiter_decrementer_node<tbb::flow::continue_msg>());


        tbb::flow::make_edge(read_file_node, limiter_input_node);
        tbb::flow::make_edge(limiter_input_node, line_buffer_node);
        tbb::flow::make_edge(line_buffer_node, double_parser_node);
        //tbb::flow::make_edge(double_parser_node, workpacket_registerer_node);
        tbb::flow::make_edge(double_parser_node, cv_fill_node);
        //tbb::flow::make_edge(cv_fill_node, workpacket_done_reporter_node);
        tbb::flow::make_edge(cv_fill_node, input_limiter_decrementer_node);
        tbb::flow::make_edge(input_limiter_decrementer_node, limiter_input_node.decrement);



        read_file_node.activate();

        g_cvdata.wait_for_all();
        //progress_printer.finish();



        return true;
    }
}