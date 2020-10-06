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
#include "../../runfile/runfile_parsed_tree.hpp"
#include "../../runfile/runfile_reweight.hpp"


namespace f = fesutils;

namespace {
    const std::string yaml_reweight_correct = "rw_all:\n"
                                              "    method: reweight_no_interp\n"
                                              "    wall_reached: discard\n"
                                              "    data: [meac, bzn, pip]\n";
    const f::RunfileItem_Reweight::ReweightMethod reweight_method_reference = f::RunfileItem_Reweight::ReweightMethod::reweight_no_interp;
    const f::RunfileItem_Reweight::WallReachedBehavior reweight_wallreached_reference = f::RunfileItem_Reweight::WallReachedBehavior::discard;
    const std::vector<std::string> reweight_data_reference = {"meac", "bzn", "pip"};

    const std::string yaml_reweight_correct_scalar_data = "rw_all:\n"
                                              "    method: reweight_no_interp\n"
                                              "    wall_reached: discard\n"
                                              "    data: pip\n";
    const std::vector<std::string> reweight_data_reference_scalar = {"pip"};

    const std::string yaml_reweight_notamap= " [a, b, c]\n";
    const std::string yaml_reweight_notamapofmap = "rw_all:\n"
                                              "    - method\n"
                                              "    - wall_reached\n"
                                              "    - data\n";

    const std::string yaml_reweight_missing_method =  "rw_all:\n"
                                                      "    wall_reached: discard\n"
                                                      "    data: [meac, bzn, pip]\n";

    const std::string yaml_reweight_method_vector = "rw_all:\n"
                                                    "    method: [reweight_no_interp, reweight_spline]\n"
                                                    "    wall_reached: discard\n"
                                                    "    data: [meac, bzn, pip]\n";

    const std::string yaml_reweight_missing_wallreached = "rw_all:\n"
                                              "    method: reweight_no_interp\n"
                                              "    data: [meac, bzn, pip]\n";

    const std::string yaml_reweight_wallreached_vector = "rw_all:\n"
                                                    "    wall_reached: [discard, ignore]\n"
                                                    "    method: reweight_no_interp\n"
                                                    "    data: [meac, bzn, pip]\n";

    const std::string yaml_reweight_missing_data = "rw_all:\n"
                                              "    method: reweight_no_interp\n"
                                              "    wall_reached: discard\n";

    const std::string yaml_reweight_map_data = "rw_all:\n"
                                                   "    method: reweight_no_interp\n"
                                                   "    wall_reached: discard\n"
                                                   "    data: {plop: a}\n";

    const std::string yaml_reweight_invalid_method = "rw_all:\n"
                                              "    method: reweight_nonexistent\n"
                                              "    wall_reached: discard\n"
                                              "    data: [meac, bzn, pip]\n";

    const std::string yaml_reweight_invalid_wallreached = "rw_all:\n"
                                              "    method: reweight_no_interp\n"
                                              "    wall_reached: notacorrectbehaviour\n"
                                              "    data: [meac, bzn, pip]\n";
}

BOOST_TEST_DONT_PRINT_LOG_VALUE(f::RunfileItem_Reweight::ReweightMethod);
BOOST_TEST_DONT_PRINT_LOG_VALUE(f::RunfileItem_Reweight::WallReachedBehavior);

BOOST_AUTO_TEST_SUITE(runfile_reweight_ts)

    BOOST_AUTO_TEST_CASE(correct_section_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        f::GeneralOptions options;

        // TEST

        YAML::Node node = YAML::Load(yaml_reweight_correct);

        f::parse_reweight_section(options, node);


        BOOST_TEST(f::g_runfileitem_reweight_list.count("rw_all") == 1);
        BOOST_REQUIRE(f::g_runfileitem_reweight_list.count("rw_all") == 1);

        f::RunfileItem_Reweight item = f::g_runfileitem_reweight_list.at("rw_all");


        BOOST_TEST(item.reweight_method == reweight_method_reference);
        BOOST_TEST(item.wall_reached_behavior == reweight_wallreached_reference);
        BOOST_TEST(item.data == reweight_data_reference);

    }

    BOOST_AUTO_TEST_CASE(correct_section_scalar_data_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        f::GeneralOptions options;

        // TEST

        YAML::Node node = YAML::Load(yaml_reweight_correct_scalar_data);

        f::parse_reweight_section(options, node);


        BOOST_TEST(f::g_runfileitem_reweight_list.count("rw_all") == 1);
        BOOST_REQUIRE(f::g_runfileitem_reweight_list.count("rw_all") == 1);

        f::RunfileItem_Reweight item = f::g_runfileitem_reweight_list.at("rw_all");

        BOOST_TEST(item.reweight_method == reweight_method_reference);
        BOOST_TEST(item.wall_reached_behavior == reweight_wallreached_reference);
        BOOST_TEST(item.data == reweight_data_reference_scalar);

    }

    BOOST_AUTO_TEST_CASE(not_a_map_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        f::GeneralOptions options;

        // TEST

        YAML::Node node = YAML::Load(yaml_reweight_notamap);

        BOOST_CHECK_THROW(f::parse_reweight_section(options, node), std::runtime_error);

        node = YAML::Load(yaml_reweight_notamapofmap);

        BOOST_CHECK_THROW(f::parse_reweight_section(options, node), std::runtime_error);

    }

    BOOST_AUTO_TEST_CASE(missing_method_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        f::GeneralOptions options;

        // TEST

        YAML::Node node = YAML::Load(yaml_reweight_missing_method);

        BOOST_CHECK_THROW(f::parse_reweight_section(options, node), std::runtime_error);

        node = YAML::Load(yaml_reweight_method_vector);

        BOOST_CHECK_THROW(f::parse_reweight_section(options, node), std::runtime_error);

    }

    BOOST_AUTO_TEST_CASE(missing_wallreached_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        f::GeneralOptions options;

        // TEST

        YAML::Node node = YAML::Load(yaml_reweight_missing_wallreached);

        BOOST_CHECK_THROW(f::parse_reweight_section(options, node), std::runtime_error);

        node = YAML::Load(yaml_reweight_wallreached_vector);

        BOOST_CHECK_THROW(f::parse_reweight_section(options, node), std::runtime_error);

    }

    BOOST_AUTO_TEST_CASE(missing_data_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        f::GeneralOptions options;

        // TEST

        YAML::Node node = YAML::Load(yaml_reweight_missing_data);

        BOOST_CHECK_THROW(f::parse_reweight_section(options, node), std::runtime_error);

        node = YAML::Load(yaml_reweight_map_data);

        BOOST_CHECK_THROW(f::parse_reweight_section(options, node), std::runtime_error);

    }

    BOOST_AUTO_TEST_CASE(invalid_method_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        f::GeneralOptions options;

        // TEST

        YAML::Node node = YAML::Load(yaml_reweight_invalid_method);

        BOOST_CHECK_THROW(f::parse_reweight_section(options, node), std::runtime_error);

    }

    BOOST_AUTO_TEST_CASE(invalid_wallreached_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        f::GeneralOptions options;

        // TEST

        YAML::Node node = YAML::Load(yaml_reweight_invalid_wallreached);

        BOOST_CHECK_THROW(f::parse_reweight_section(options, node), std::runtime_error);

    }


BOOST_AUTO_TEST_SUITE_END();