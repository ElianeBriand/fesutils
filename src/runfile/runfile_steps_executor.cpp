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
#include "runfile_steps_executor.hpp"
#include "runfile_parsed_tree.hpp"
#include "../steps/step_fes_data.hpp"
#include "../frontend_common.h"

#include <boost/log/trivial.hpp>


namespace fesutils {

    // LCOV_EXCL_START
    // Reason for coverage exclusion: Would in essence be an integration test, not unit test
    void execute_steps(GeneralOptions& options) {

        const size_t num_steps = g_all_steps_list.size();


        for(size_t i = 0; i < num_steps; i++) {
            RunfileItem_Step& current_step = g_all_steps_list[i];

            for(const std::string parameter_set_name: current_step.parameter_set_names) {
                BOOST_LOG_TRIVIAL(info) << "Executing step [ " << stepfunction_to_string(current_step.function)
                << " ] with parameter set [ " << parameter_set_name << " ]";

                setup_log_formating_increase_indentation();

                if(current_step.function == StepFunction::fes_data) {
                    run_fes_data_step(options, parameter_set_name);
                } else {
                    BOOST_LOG_TRIVIAL(debug) << "[NOT IMPLEMENTED] step not implemented";
                }

                setup_log_formating_decrease_indentation();

                BOOST_LOG_TRIVIAL(info) << "Finished executing step [ " << stepfunction_to_string(current_step.function)
                                        << " ] with parameter set [ " << parameter_set_name << " ]";
                BOOST_LOG_TRIVIAL(info) << "\n\n";

            }



        }
    }
    // LCOV_EXCL_STOP

}