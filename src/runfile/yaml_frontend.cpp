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

#include "yaml_frontend.hpp"
#include "runfile_general_section.hpp"
#include "runfile_search_directory_section.hpp"
#include "runfile_steps.hpp"
#include "runfile_fes_data.hpp"
#include "runfile_reweight.hpp"
#include "runfile_steps_executor.hpp"

#include <boost/log/trivial.hpp>
#include <yaml-cpp/yaml.h>



namespace fesutils {
        void read_yaml_runfile(GeneralOptions options,
        const std::string& runfile_path) {


            YAML::Node runfile_yaml = YAML::LoadFile(runfile_path);


            if (runfile_yaml["general"]) {

                YAML::Node general_node = runfile_yaml["general"];

                parse_general_section(options, general_node);



            } else {
                BOOST_LOG_TRIVIAL(error) << "Missing mandatory section in YAML runfile: general";
                throw std::runtime_error("Missing mandatory YAML runfile section");
            }

            if (runfile_yaml["search_directory"]) {

                YAML::Node search_directory_node = runfile_yaml["search_directory"];

                parse_search_directory_section(options, search_directory_node);

            } else {
                BOOST_LOG_TRIVIAL(error) << "Missing mandatory section in YAML runfile: search_directory";
                throw std::runtime_error("Missing mandatory YAML runfile section");
            }

            if (runfile_yaml["fes_data"]) {

                YAML::Node fes_data_node = runfile_yaml["fes_data"];

                parse_fes_data_section(options, fes_data_node);

            } else {
                BOOST_LOG_TRIVIAL(error) << "Missing mandatory section in YAML runfile: fes_data";
                throw std::runtime_error("Missing mandatory YAML runfile section");
            }

            if (runfile_yaml["reweight"]) {

                YAML::Node reweight_node = runfile_yaml["reweight"];

                parse_reweight_section(options, reweight_node);

            }

            if (runfile_yaml["histogram"]) {

                YAML::Node histogram_node = runfile_yaml["histogram"];
                BOOST_LOG_TRIVIAL(debug) << "[NOT IMPLEMENTED] A histogram section was encountered, but it is not implemented yet";
                // TODO: parse_histogram_section(options, histogram_node);

            }

            if (runfile_yaml["reconstruction"]) {

                YAML::Node reconstruction_node = runfile_yaml["reconstruction"];
                BOOST_LOG_TRIVIAL(debug) << "[NOT IMPLEMENTED] A reconstruction section was encountered, but it is not implemented yet";
                // TODO: parse_reconstruction_section(options, reconstruction_node);

            }



            if (runfile_yaml["steps"]) {

                YAML::Node steps_node = runfile_yaml["steps"];

                parse_steps_section(options, steps_node);

            } else {
                BOOST_LOG_TRIVIAL(error) << "Missing mandatory section in YAML runfile: steps";
                throw std::runtime_error("Missing mandatory YAML runfile section");
            }



        }
}



