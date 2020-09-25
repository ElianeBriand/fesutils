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


#include "runfile_reweight.hpp"
#include "runfile_parsed_tree.hpp"
#include "../common/yaml_utils.hpp"


namespace fesutils {

    void parse_reweight_section(GeneralOptions& options, YAML::Node& reweight_node) {

        if(!reweight_node.IsMap()) {
            BOOST_LOG_TRIVIAL(error) << "The reweight section should be a map of map (error code 00103)";
            throw std::runtime_error("Runfile structure error");
        }

        for(YAML::const_iterator node_it = reweight_node.begin();
            node_it != reweight_node.end();
            node_it++) {

            const std::string item_shorthand = node_it->first.as<std::string>();

            if(!node_it->second.IsMap()) {
                BOOST_LOG_TRIVIAL(error) << "In reweight section, for item: "  << item_shorthand;
                BOOST_LOG_TRIVIAL(error) << "Each subsection of the reweight map should be a map. (error code 00103)";
                throw std::runtime_error("Runfile structure error");
            }

            const YAML::Node& fes_data_item_node = node_it->second;

            if (!fes_data_item_node["method"]) {
                if (!fes_data_item_node["method"].IsScalar() ) {
                    BOOST_LOG_TRIVIAL(error) << "In fes_data section, for item: "  << item_shorthand;
                    BOOST_LOG_TRIVIAL(error) << "The method attribute should be a scalar corresponding to the reweight method (error code 00103)";
                    throw std::runtime_error("Runfile structure error");
                }
                BOOST_LOG_TRIVIAL(error) << "In reweight section, for item: "  << item_shorthand;
                BOOST_LOG_TRIVIAL(error) << "Missing mandatory property: method (error code 00103)";
                throw std::runtime_error("Missing mandatory YAML runfile section");
            }

            if (!fes_data_item_node["wall_reached"]) {
                if (!fes_data_item_node["wall_reached"].IsScalar() ) {
                    BOOST_LOG_TRIVIAL(error) << "In fes_data section, for item: "  << item_shorthand;
                    BOOST_LOG_TRIVIAL(error) << "The wall_reached attribute should be a scalar corresponding to the action to be taken (error code 00103)";
                    throw std::runtime_error("Runfile structure error");
                }
                BOOST_LOG_TRIVIAL(error) << "In reweight section, for item: "  << item_shorthand;
                BOOST_LOG_TRIVIAL(error) << "Missing mandatory property: wall_reached (error code 00103)";
                throw std::runtime_error("Missing mandatory YAML runfile section");
            }

            if (!fes_data_item_node["data"]) {
                BOOST_LOG_TRIVIAL(error) << "In fes_data section, for item: "  << item_shorthand;
                BOOST_LOG_TRIVIAL(error) << "Missing mandatory property: data (error code 00103)";
                throw std::runtime_error("Missing mandatory YAML runfile section");
            }

            if (!fes_data_item_node["data"].IsScalar() && !fes_data_item_node["data"].IsSequence() ) {
                BOOST_LOG_TRIVIAL(error) << "In fes_data section, for item: "  << item_shorthand;
                BOOST_LOG_TRIVIAL(error) << "The data attribute should be either a shorthand or a list of shorthand (referencing fes_data shorthand) (error code 00103)";
                throw std::runtime_error("Runfile structure error");
            }


            RunfileItem_Reweight curr_item;

            std::vector<std::string> fes_data_shorthands;
            if(fes_data_item_node["data"].IsScalar())
            {
                fes_data_shorthands.push_back(fes_data_item_node["data"].as<std::string>());
            }else {
                const YAML::Node& fes_data_vector_node = fes_data_item_node["data"];
                fes_data_shorthands = string_vector_from_yaml_sequence_node(fes_data_vector_node);
            }

            const std::string method_str = fes_data_item_node["method"].as<std::string>();
            if(method_str == "reweight_no_interp") {
                curr_item.reweight_method = RunfileItem_Reweight::reweight_no_interp;
            } else {
                BOOST_LOG_TRIVIAL(error) << "The specified reweighting method  is unknown (error code 00103)";
                throw std::runtime_error("Unknown method attribute");
            }


            const std::string wall_reached_b_str = fes_data_item_node["wall_reached"].as<std::string>();
            if(wall_reached_b_str == "discard") {
                curr_item.wall_reached_behavior = RunfileItem_Reweight::discard;
            } else {
                BOOST_LOG_TRIVIAL(error) << "The specified wall_reached behaviour is unknown (error code 00103)";
                throw std::runtime_error("Unknown wall_reached behaviour");
            }

            g_runfileitem_reweight_list[item_shorthand] = curr_item;

        }
    }

}