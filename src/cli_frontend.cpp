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
#include "help/help.hpp"

namespace po = boost::program_options;

namespace f = fesutils;



int main(int argc, char** argv) {

    f::GeneralOptions options;

    po::options_description global_options("Global options");
    global_options.add_options()
            ("debug", "Turn on debug output")
            ("quiet", "Disable all output except errors")
            ;

    po::options_description positional_workaround_options("Global options");
    positional_workaround_options.add_options()
            ("command", po::value<std::string>(), "command to execute")
            ("subargs", po::value<std::vector<std::string> >(), "Arguments for command");



    po::positional_options_description pos;
    pos.add("command", 1).
            add("subargs", -1);

    po::variables_map vm;

    po::options_description cmdline_options;
    cmdline_options.add(global_options).add(positional_workaround_options);

    po::parsed_options parsed = po::command_line_parser(argc, argv).
            options(cmdline_options).
            positional(pos).
            allow_unregistered().
            run();



    po::store(parsed, vm);

    if (vm.count("debug")) {
        BOOST_LOG_TRIVIAL(debug) << "Debug messages enabled";
        setupLogPrinting(false, false);
    } else if (vm.count("quiet")) {
        setupLogPrinting(false, true);
    } else {
        setupLogPrinting(true, false);
    }

    BOOST_LOG_TRIVIAL(info) << "";
    BOOST_LOG_TRIVIAL(info) << "fesutils v0.1";
    BOOST_LOG_TRIVIAL(info) << "Copyright (C) 2010  Eliane Briand";
    BOOST_LOG_TRIVIAL(info) << "    This program comes with ABSOLUTELY NO WARRANTY.";
    BOOST_LOG_TRIVIAL(info) << "    This is free software, and you are welcome to redistribute it.";
    BOOST_LOG_TRIVIAL(info) << "    You should have received a copy of the GNU General Public License version 3";
    BOOST_LOG_TRIVIAL(info) << "    along with this program.  If not, see <https://www.gnu.org/licenses/>.\n\n";




    if (vm.count("command")) {
        std::string cmd = vm["command"].as<std::string>();

        if (cmd == "final_bias_reweight") {

            po::options_description fbw_desc("final_bias_reweight options");
            fbw_desc.add_options()
                    // ("nospline", "Show hidden files")
                    ("cvfile", po::value<std::string>(), "File with the MetaD CV computed on the trajectory.")
                    ("outfile", po::value<std::string>(), "File to output the CV alongside their computed weight.")
                    ("metadgrid", po::value<std::string>(),
                     "File with the MetaD grid potential, typically dumped with WGRID_FILE")
                    ("biasfield", po::value<std::string>(),
                     "Name of the field corresponding to the bias value")
                    ("ranges_min", po::value<std::string>(),
                     "Minimal threhold value of the variable for the frame to be considered. Can be multiple values: 0.0,1.2,3.1.")
                    ("ranges_max", po::value<std::string>(),
                     "Max threhold value of the variable for the frame to be considered.");

            // Erases the already parsed options (including command)
            std::vector<std::string> opts = po::collect_unrecognized(parsed.options, po::include_positional);
            opts.erase(opts.begin());

            // Parse again...
            po::store(po::command_line_parser(opts).options(fbw_desc).run(), vm);

            if( !(vm.count("cvfile")) ||
                !(vm.count("outfile")) ||
                !(vm.count("metadgrid"))
                ) {
                BOOST_LOG_TRIVIAL(error) << "Missing mandatory parameters for final_bias_reweight: --cvfile, --outfile and/or --metadgrid";
                BOOST_LOG_TRIVIAL(error) << fbw_desc;

            }else {
                f::final_bias_reweight_args args;

                if(vm.count("ranges_min")) {
                    args.ranges_min_rawstr = vm["ranges_min"].as<std::string>();
                }

                if(vm.count("ranges_max")) {
                    args.ranges_max_rawstr = vm["ranges_min"].as<std::string>();
                }

                args.cvfile_path = vm["cvfile"].as<std::string>();

                args.metadgrid_path = vm["metadgrid"].as<std::string>();

                f::do_final_bias_reweight(options, args);

                return 0;
            }
        }else if (cmd == "help") {
            po::positional_options_description pos_help;
            pos_help.add("which_command", 1);


            // Erases the already parsed options (including command)
            std::vector<std::string> opts = po::collect_unrecognized(parsed.options, po::include_positional);
            opts.erase(opts.begin());

            // Parse again...
            po::store(po::command_line_parser(opts).positional(pos_help).run(), vm);

            if(!vm.count("which_command")) {
                BOOST_LOG_TRIVIAL(error) << "Specify topic/command for help: " << argv[0] << " help command_or_topic";
                std::exit(1);
            }
            std::string which_cmd = vm["which_command"].as<std::string>();

            f::do_help(which_cmd);
        }else {
            BOOST_LOG_TRIVIAL(error) << "Unrecognised command: " << cmd;
        }
    }

    BOOST_LOG_TRIVIAL(info) << global_options << "\n";
    BOOST_LOG_TRIVIAL(info) << "\n\n";
    BOOST_LOG_TRIVIAL(info) << argv[0] << " subcommand --args1 --args2";
    BOOST_LOG_TRIVIAL(info) << "\n\n";
    BOOST_LOG_TRIVIAL(info) << "Supported subcommand are: ";
    BOOST_LOG_TRIVIAL(info) << "final_bias_reweight - Reweight frames from a MetaD trajectory using a final bias grid.";
    return 1;

}
