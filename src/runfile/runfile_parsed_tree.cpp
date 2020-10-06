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

#include "runfile_parsed_tree.hpp"

#include <boost/log/trivial.hpp>


namespace fesutils {



    std::vector<RunfileItem_Step> g_all_steps_list;
    std::vector<RunfileItem_Step> g_remaining_steps_lists;

    std::map<std::string, RunfileItem_FESData> g_runfileitem_fesdata_list;
    std::map<std::string, RunfileItem_Reweight> g_runfileitem_reweight_list;


    StepFunction string_to_stepfunction(const std::string& function_as_string) {
        if(function_as_string == "fes_data") {
            return StepFunction::fes_data;
        } else if (function_as_string == "reweight") {
            return StepFunction::reweight;
        } else if (function_as_string == "histogram") {
            return StepFunction::histogram;
        } else if (function_as_string == "reconstruction") {
            return StepFunction::reconstruction;
        } else {
            BOOST_LOG_TRIVIAL(error) << "Unrecognized step function: " << function_as_string;
            throw std::runtime_error("Unknwon step function");
        }

    }

    std::string stepfunction_to_string(const StepFunction& stepFunction) {
        if(stepFunction == StepFunction::fes_data) {
            return "fes_data";
        } else if (stepFunction == StepFunction::reweight) {
            return "reweight";
        } else if (stepFunction == StepFunction::histogram) {
            return "histogram";
        } else if (stepFunction == StepFunction::reconstruction) {
            return "reconstruction";
        } else {
            // LCOV_EXCL_START
            // Reason for coverage exclusion: Difficult to generate incorrect values for enum class
            BOOST_LOG_TRIVIAL(error) << "Unrecognized step function enum value";
            BOOST_LOG_TRIVIAL(error) << "Likely a bug rather than a user error";
            throw std::runtime_error("Unknown step function enum value");
            // LCOV_EXCL_STOP
        }
    }

}


