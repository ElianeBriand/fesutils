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
#include "../../GeneralOptions.h"
#include "../../runfile/runfile_search_directory_section.hpp"


namespace f = fesutils;

namespace {
    const std::string yaml_directory_correct = "all: \"./examples/\"\n";
    const std::string yaml_directory_noalldirspecified = "other: \"./examples/\"\n";
}

BOOST_AUTO_TEST_SUITE(runfile_search_directory_ts)

    BOOST_AUTO_TEST_CASE(all_dir_specificied_tc, *boost::unit_test::tolerance(0.001)) {

        // PRELUDE
        f::GeneralOptions options;

        // TEST

        YAML::Node node = YAML::Load(yaml_directory_correct);

        f::parse_search_directory_section(options, node);

        BOOST_TEST(options.search_directory.count("all") == 1);
        BOOST_REQUIRE(options.search_directory.count("all") == 1);

        BOOST_TEST(options.search_directory.at("all") == "./examples/");


    }

    BOOST_AUTO_TEST_CASE(all_dir_not_specific_tc, *boost::unit_test::tolerance(0.001)) {

        // PRELUDE
        f::GeneralOptions options;

        // TEST

        YAML::Node node = YAML::Load(yaml_directory_noalldirspecified);

        f::parse_search_directory_section(options, node);

        BOOST_TEST(options.search_directory.count("all") == 1);
        BOOST_REQUIRE(options.search_directory.count("all") == 1);

        BOOST_TEST(options.search_directory.at("all") == "./");


    }


BOOST_AUTO_TEST_SUITE_END();