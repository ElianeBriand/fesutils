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

#include "yaml_utils.hpp"

#include <boost/log/trivial.hpp>


namespace fesutils {

    std::vector<std::string> string_vector_from_yaml_sequence_node(const YAML::Node& seq_node) {
        std::vector<std::string> str_vec;
        if(!seq_node.IsSequence()) {
            BOOST_LOG_TRIVIAL(error) << "string_vector_from_yaml_sequence_node expects a YAML sequence node as input ";
            BOOST_LOG_TRIVIAL(error) << "This is a bug, not a user error.";
            throw std::runtime_error("string_vector_from_yaml_sequence_node expects a YAML sequence node");
        }
        str_vec = seq_node.as<std::vector<std::string>>();

        return str_vec;
    }
}