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

#include <string>

#include <boost/log/trivial.hpp>


#include <boost/program_options.hpp>

#include "GeneralOptions.h"
#include "final_bias_reweight/final_bias_reweight.hpp"
#include "frontend_common.h"
#include "reconstruct_from_fragments/reconstruct_from_fragments.hpp"
#include "runfile/yaml_frontend.hpp"
#include "runfile/runfile_steps_executor.hpp"


namespace po = boost::program_options;

namespace f = fesutils;


// LCOV_EXCL_START
// Reason for coverage exclusion: System testing cover this

int main(int argc, char** argv) {

    setup_log_formating();
    setup_log_level_filter(true, false);

    f::GeneralOptions options;


    po::options_description positional_workaround_options("Arguments");
    positional_workaround_options.add_options()
            ("RUNFILE", po::value<std::string>(), "YAML runfile describing steps & parameters");

    po::positional_options_description pos;
    pos.add("RUNFILE", 1);

    po::variables_map vm;

    po::options_description cmdline_options;
    cmdline_options.add(positional_workaround_options);

    po::parsed_options parsed = po::command_line_parser(argc, argv).
            options(cmdline_options).
            positional(pos).
            run();

    po::store(parsed, vm);

    if(!vm.count("RUNFILE")) {
        BOOST_LOG_TRIVIAL(info) << "Please add a runfile argument.";
        return 1;
    }

    std::string runfile_path = vm["RUNFILE"].as<std::string>();

    f::read_yaml_runfile(options, runfile_path);




    BOOST_LOG_TRIVIAL(info) << "";
    BOOST_LOG_TRIVIAL(info) << "fesutils v0.1";
    BOOST_LOG_TRIVIAL(info) << "";
    BOOST_LOG_TRIVIAL(info) << "This program comes with ABSOLUTELY NO WARRANTY.";
    BOOST_LOG_TRIVIAL(info) << "This is free software, and you are welcome to redistribute it.";
    BOOST_LOG_TRIVIAL(info) << "You should have received a copy of the GNU General Public License version 3";
    BOOST_LOG_TRIVIAL(info) << "along with this program.  If not, see <https://www.gnu.org/licenses/>.\n\n";


    f::execute_steps(options);

    return 0;

}

// LCOV_EXCL_STOP
