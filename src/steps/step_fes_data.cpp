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
#include "step_fes_data.hpp"
#include "../runfile/runfile_parsed_tree.hpp"
#include "../file_reader/read_file_header.hpp"
#include "../file_reader/cv_file/CVData.hpp"
#include "../file_reader/cv_file/ingest_cv_data.hpp"
#include "../frontend_common.h"
#include "../file_reader/grid_file/ingest_grid_data.hpp"
#include "../file_reader/field_function_heuristics.hpp"


namespace fesutils {

    // LCOV_EXCL_START
    // Reason for coverage exclusion: Would in essence be an integration test, not unit test

    bool run_fes_data_step(GeneralOptions options, const std::string& parameter_set_name) {

        RunfileItem_FESData fes_data_runfile = g_runfileitem_fesdata_list[parameter_set_name];

        const std::string cv_file_path_str = fes_data_runfile.cv_file_path.generic_string();

        const unsigned int num_cv_dimensions = fes_data_runfile.cv_varnames.size();

        // 1 for the metad bias, 1 for each other value of interest (wall, etc)
        const unsigned int num_value_dimensions = 1 +  fes_data_runfile.wall_bias_varnames.size();

        BOOST_LOG_TRIVIAL(info) << "Reading header from " << cv_file_path_str;
        PlumedDatHeader cv_header = read_plumed_file_header(cv_file_path_str);

        std::shared_ptr<CVData> cv_data = CVData_factory(CVData_storage_class::inmemory,
                                                         num_cv_dimensions,
                                                         num_value_dimensions,
                                                         1000); // Start with one thousand record

        BOOST_LOG_TRIVIAL(info) << "Preparing to read CV data from " << cv_file_path_str;
        setup_log_formating_increase_indentation();
        bool ingest_cv_success = ingest_cv_data(options,
                                                fes_data_runfile.cv_file_path,
                                                cv_data,
                                                cv_header,
                                                fes_data_runfile.cv_varnames,
                                                fes_data_runfile.metad_bias_varname,
                                                fes_data_runfile.wall_bias_varnames);
        setup_log_formating_decrease_indentation();
        if(ingest_cv_success) {
            BOOST_LOG_TRIVIAL(info) << "CV data successfully read";
        } else {
            BOOST_LOG_TRIVIAL(error) << "CV data unsuccessfully read";
            throw std::runtime_error("Could not read CV data");
        }

        if(fes_data_runfile.grid_file_path.has_value()) {

            BOOST_LOG_TRIVIAL(info) << "GRID data present, using it instead of HILLS data ";
            const std::string grid_file_path_str = fes_data_runfile.grid_file_path.value().generic_string();
            BOOST_LOG_TRIVIAL(info) << "Preparing to read GRID data from " << grid_file_path_str;

            PlumedDatHeader header_grid = read_plumed_file_header(
                    fes_data_runfile.grid_file_path.value().generic_string());


            size_t metad_bias_var_index = header_grid.find_field_index_from_name(fes_data_runfile.metad_bias_varname);

            std::vector<size_t> variable_field_indexes;
            for(const std::string& cv_varname_str: fes_data_runfile.cv_varnames) {
                size_t var_idx = header_grid.find_field_index_from_name(cv_varname_str);
                variable_field_indexes.push_back(var_idx);
            }

            setup_log_formating_increase_indentation();

            std::shared_ptr<GridData> grid = ingest_grid_data(options,
                                                      fes_data_runfile.grid_file_path.value(),
                                                      header_grid,
                                                      metad_bias_var_index,
                                                      variable_field_indexes
                                                      );

            setup_log_formating_decrease_indentation();

            if(grid != nullptr) {
                BOOST_LOG_TRIVIAL(info) << "GRID data successfully read";
            } else {
                BOOST_LOG_TRIVIAL(error) << "GRID data unsuccessfully read";
                throw std::runtime_error("Could not read GRID data");
            }

        } else if(fes_data_runfile.hills_file_path.has_value()) {

            BOOST_LOG_TRIVIAL(info) << "GRID data not present, using HILLS data instead";
            const std::string hills_file_path_str = fes_data_runfile.hills_file_path.value().generic_string();
            BOOST_LOG_TRIVIAL(info) << "Preparing to read HILLS data from " << hills_file_path_str;


        } else {

            BOOST_LOG_TRIVIAL(error) << "Can find a path to neither HILLS or GRID data for fes_data step <" << parameter_set_name << ">";
            throw std::runtime_error("Neither HILLS nor GRID found in fes_data step");
        }



        return true;
    }

    // LCOV_EXCL_STOP
}