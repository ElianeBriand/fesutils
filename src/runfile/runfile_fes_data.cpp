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

#include "runfile_fes_data.hpp"

#include "runfile_parsed_tree.hpp"
#include "../common/find_in_search_directory.hpp"
#include "../common/yaml_utils.hpp"

#include <boost/log/trivial.hpp>


namespace fesutils {

    void parse_fes_data_section(GeneralOptions& options, YAML::Node& fes_data_node) {
        if(!fes_data_node.IsMap()) {
            BOOST_LOG_TRIVIAL(error) << "The fes_data section should be a map of map (error code 00101)";
            throw std::runtime_error("Runfile structure error.");
        }
        for(YAML::const_iterator node_it = fes_data_node.begin();
            node_it != fes_data_node.end();
            node_it++) {

            const std::string item_shorthand = node_it->first.as<std::string>();

            if(!node_it->second.IsMap()) {
                BOOST_LOG_TRIVIAL(error) << "In fes_data section, for item: "  << item_shorthand;
                BOOST_LOG_TRIVIAL(error) << "Each subsection of the fes_data map should be a map. (error code 00101)";
                throw std::runtime_error("Runfile structure error.");
            }

            const YAML::Node& fes_data_item_node = node_it->second;

            if (!fes_data_item_node["name"]) {
                BOOST_LOG_TRIVIAL(error) << "In fes_data section, for item: "  << item_shorthand;
                BOOST_LOG_TRIVIAL(error) << "Missing mandatory property: name (error code 00102)";
                throw std::runtime_error("Missing mandatory YAML runfile section");
            }

            if (!fes_data_item_node["cv_filename_hint"]) {
                BOOST_LOG_TRIVIAL(error) << "In fes_data section, for item: "  << item_shorthand;
                BOOST_LOG_TRIVIAL(error) << "Missing mandatory property: cv_filename_hint (error code 00102)";
                throw std::runtime_error("Missing mandatory YAML runfile section");
            }

            if (!fes_data_item_node["hills_filename_hint"] && !fes_data_item_node["grid_filename_hint"]) {
                BOOST_LOG_TRIVIAL(error) << "In fes_data section, for item: "  << item_shorthand;
                BOOST_LOG_TRIVIAL(error) << "Missing mandatory property: hills_filename_hint or grid_filename_hint (error code 00102)";
                throw std::runtime_error("Missing mandatory YAML runfile section");
            }

            if (!fes_data_item_node["cv_varnames"]) {
                BOOST_LOG_TRIVIAL(error) << "In fes_data section, for item: "  << item_shorthand;
                BOOST_LOG_TRIVIAL(error) << "Missing mandatory property: cv_varnames (error code 00102)";
                throw std::runtime_error("Missing mandatory YAML runfile section");
            }

            if (!fes_data_item_node["metad_bias_varname"]) {
                BOOST_LOG_TRIVIAL(error) << "In fes_data section, for item: "  << item_shorthand;
                BOOST_LOG_TRIVIAL(error) << "Missing mandatory property: metad_bias_varname (error code 00102)";
                throw std::runtime_error("Missing mandatory YAML runfile section");
            }

            if(!fes_data_item_node["metad_bias_varname"].IsScalar()) {
                BOOST_LOG_TRIVIAL(error) << "In fes_data section, for item: "  << item_shorthand;
                BOOST_LOG_TRIVIAL(error) << "Property metad_bias_varname expect a single value - the name of the metadynamic bias variable (error code 00102)";
                BOOST_LOG_TRIVIAL(error) << "Possibly a vector/sequence was entered where a scalar value was expected";
                throw std::runtime_error("metad_bias_varname is not a scalar");
            }


            RunfileItem_FESData curr_item;
            curr_item.name = fes_data_item_node["name"].as<std::string>();
            curr_item.cv_filename_hint = fes_data_item_node["cv_filename_hint"].as<std::string>();


            curr_item.cv_file_path = find_cv_path_from_hint(options, curr_item.cv_filename_hint);

            if(fes_data_item_node["hills_filename_hint"]) {
                curr_item.hills_filename_hint = fes_data_item_node["hills_filename_hint"].as<std::string>();
                curr_item.hills_file_path = find_hills_path_from_hint(options, curr_item.hills_filename_hint);
            }

            if(fes_data_item_node["grid_filename_hint"]) {
                curr_item.grid_filename_hint = fes_data_item_node["grid_filename_hint"].as<std::string>();
                curr_item.grid_file_path = find_grid_path_from_hint(options, curr_item.grid_filename_hint);
            }



            const YAML::Node& metad_bias_varname_node = fes_data_item_node["metad_bias_varname"];

            curr_item.metad_bias_varname = metad_bias_varname_node.as<std::string>();

            const YAML::Node& cv_varnames_node = fes_data_item_node["cv_varnames"];
            curr_item.cv_varnames = string_vector_from_yaml_sequence_node(cv_varnames_node);

            if(fes_data_item_node["wall_bias_varnames"]) {
                const YAML::Node& wall_bias_varnames_node = fes_data_item_node["wall_bias_varnames"];
                curr_item.wall_bias_varnames = string_vector_from_yaml_sequence_node(wall_bias_varnames_node);
            }

            g_runfileitem_fesdata_list[item_shorthand] = curr_item;

        }

    }

}