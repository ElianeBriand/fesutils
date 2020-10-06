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

#ifndef FESUTILS_RUNFILE_PARSED_TREE_HPP
#define FESUTILS_RUNFILE_PARSED_TREE_HPP

#include <vector>

#include <boost/filesystem.hpp>
namespace bf = boost::filesystem;

#include "../final_bias_reweight/final_bias_reweight.hpp"




namespace fesutils {

    struct RunfileItem_FESData {
        std::string name;
        std::string cv_filename_hint;
        std::string hills_filename_hint;
        std::string grid_filename_hint;

        bf::path cv_file_path;
        std::optional<bf::path> hills_file_path;
        std::optional<bf::path> grid_file_path;

        std::vector<std::string> cv_varnames;
        std::vector<std::string> wall_bias_varnames;
        std::string metad_bias_varname;
    };

   extern std::map<std::string, RunfileItem_FESData> g_runfileitem_fesdata_list;



   struct RunfileItem_Reweight {
        enum class WallReachedBehavior {
           discard
        };

       enum class ReweightMethod {
           // reweight_spline, // not implemented yet
           reweight_no_interp
       };

        WallReachedBehavior wall_reached_behavior;
        ReweightMethod reweight_method;
        std::vector<std::string> data;
        std::vector<final_bias_reweight_args> args_v;

    };

    extern std::map<std::string, RunfileItem_Reweight> g_runfileitem_reweight_list;

    enum class StepFunction {
        fes_data,
        reweight,
        histogram,
        reconstruction
    };
    StepFunction string_to_stepfunction(const std::string& function_as_string);
    std::string stepfunction_to_string(const StepFunction& stepFunction);

    struct RunfileItem_Step {
        StepFunction function;
        std::vector<std::string> parameter_set_names;
    };

    extern std::vector<RunfileItem_Step> g_all_steps_list;
    extern std::vector<RunfileItem_Step> g_remaining_steps_lists;

}

#endif //FESUTILS_RUNFILE_PARSED_TREE_HPP
