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

#define BOOST_TEST_DYN_LINK

#include <vector>
#include <iostream>

#include <boost/test/unit_test.hpp>
#include "../../common/yaml_utils.hpp"


namespace f = fesutils;

BOOST_AUTO_TEST_SUITE(yaml_utils_ts)

    BOOST_AUTO_TEST_CASE(yaml_utils_tc, *boost::unit_test::tolerance(0.001)) {
        YAML::Node node = YAML::Load("[aaa, bbb, ccc]");

        BOOST_REQUIRE(node.Type() == YAML::NodeType::Sequence);
        BOOST_REQUIRE(node.IsSequence());

        std::vector<std::string> res = f::string_vector_from_yaml_sequence_node(node);

        BOOST_TEST(res.size() == 3);
        BOOST_REQUIRE(res.size() == 3);
        BOOST_TEST(res[0] == "aaa");
        BOOST_TEST(res[1] == "bbb");
        BOOST_TEST(res[2] == "ccc");

    }

    BOOST_AUTO_TEST_CASE(yaml_utils_check_throw_tc, *boost::unit_test::tolerance(0.001)) {

        YAML::Node node = YAML::Load("aaa");

        BOOST_REQUIRE(node.Type() != YAML::NodeType::Sequence);
        BOOST_REQUIRE(!node.IsSequence());

        BOOST_CHECK_THROW(f::string_vector_from_yaml_sequence_node(node);, std::runtime_error);


    }


BOOST_AUTO_TEST_SUITE_END();