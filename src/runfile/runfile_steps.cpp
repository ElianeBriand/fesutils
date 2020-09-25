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

#include "runfile_steps.hpp"
#include "../common/yaml_utils.hpp"
#include "runfile_parsed_tree.hpp"


namespace fesutils {

    void parse_steps_section(GeneralOptions& options, YAML::Node& steps_node) {
        if(!steps_node.IsSequence()) {
            BOOST_LOG_TRIVIAL(error) << "The step section must be a sequence of map (error code 00104)";
            throw std::runtime_error("Runfile structure error");
        }

        for (std::size_t i = 0; i< steps_node.size(); i++) {
            if(!steps_node[i].IsMap()) {
                BOOST_LOG_TRIVIAL(error) << "The step section must be a sequence of map (error code 00104)";
                throw std::runtime_error("Runfile structure error");
            }
            const std::string function_str = steps_node[i].begin()->first.as<std::string>();
            std::vector<std::string> parameter_sets;
            if(steps_node[i].begin()->second.IsSequence()) {
                parameter_sets = string_vector_from_yaml_sequence_node(steps_node[i].begin()->second);
            } else if(steps_node[i].begin()->second.IsScalar()) {
                const std::string parameter_str = steps_node[i].begin()->second.as<std::string>();
                parameter_sets.push_back(parameter_str);
            } else {
                BOOST_LOG_TRIVIAL(error) << "Unexpected step structure (error code 00104)";
                throw std::runtime_error("Runfile structure error");
            }
            BOOST_LOG_TRIVIAL(debug) << "Registered steps: function = " << function_str << " | params = " << parameter_sets;

            RunfileItem_Step curr_step;

            curr_step.function = string_to_stepfunction(function_str);

            for(const std::string& param_set: parameter_sets) {
                if(!check_if_parameter_sets_exists(curr_step.function, param_set)) {
                    BOOST_LOG_TRIVIAL(error) << "Parameter set " << param_set << " for function " << function_str <<
                    " has not been declared. Check input file for naming mistake.";
                    throw std::runtime_error("Unknwon parameter set for function");
                }
            }
            curr_step.parameter_set_names = parameter_sets;

            g_all_steps_list.push_back(curr_step);
            g_remaining_steps_lists.push_back(curr_step);
        }



    }

    bool check_if_parameter_sets_exists(const StepFunction function, const std::string& param_set_str) {
        if(function == fes_data) {
            return g_runfileitem_fesdata_list.count(param_set_str);
        } else if (function == reweight) {
            return g_runfileitem_reweight_list.count(param_set_str);
        } else {
            BOOST_LOG_TRIVIAL(error) << "Unrecognized step function enum value: " << function;
            BOOST_LOG_TRIVIAL(error) << "This is likely a bug, not an user error";
            throw std::runtime_error("Unknwon step function");
        }
    }

}