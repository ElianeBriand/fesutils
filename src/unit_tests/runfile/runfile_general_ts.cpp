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
#include "../../runfile/runfile_general_section.hpp"
#include "../../GeneralOptions.h"

namespace f = fesutils;

BOOST_TEST_DONT_PRINT_LOG_VALUE(f::LogLevel);
BOOST_TEST_DONT_PRINT_LOG_VALUE(f::BinaryCachePolicy);

namespace {
    const std::string yaml_general_correct = "title: \"Reweight & fragment reconstruction\"\n"
                                             "log_level: debug\n"
                                             "binary_cache_policy: always\n"
                                             "ui_bling: true # Enables progress bar & co. Disable if non-interactive batch";
    const std::string general_title_reference = "Reweight & fragment reconstruction";
    const f::LogLevel general_loglevel_reference = f::LogLevel::debug;
    const f::BinaryCachePolicy general_bincachepol_reference = f::BinaryCachePolicy::always;
    const bool general_ui_bling_reference = true;

    const std::string yaml_general_notitle = "log_level: debug\n"
                                             "binary_cache_policy: always\n"
                                             "ui_bling: true # Enables progress bar & co. Disable if non-interactive batch";
    const std::string general_title_default = "Untitled run";

    const std::string yaml_general_nologlevel = "title: \"Reweight & fragment reconstruction\"\n"
                                                "binary_cache_policy: always\n"
                                                "ui_bling: true # Enables progress bar & co. Disable if non-interactive batch";
    const f::LogLevel general_loglevel_default =  f::LogLevel::info;

    const std::string yaml_general_nobincachepol = "title: \"Reweight & fragment reconstruction\"\n"
                                                "log_level: debug\n"
                                                "ui_bling: true # Enables progress bar & co. Disable if non-interactive batch";
    const f::BinaryCachePolicy general_bincachepol_default =  f::BinaryCachePolicy::never;

    const std::string yaml_general_nouibling = "title: \"Reweight & fragment reconstruction\"\n"
                                                "binary_cache_policy: always\n"
                                               "log_level: debug\n";
    const bool general_uibling_default =  false;
}

BOOST_AUTO_TEST_SUITE(runfile_general_ts)

    BOOST_AUTO_TEST_CASE(correct_block_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        f::GeneralOptions options;

        // TEST

        YAML::Node node = YAML::Load(yaml_general_correct);

        f::parse_general_section(options, node);

        BOOST_TEST(options.run_title == general_title_reference);

        BOOST_TEST(options.loglevel == general_loglevel_reference);

        BOOST_TEST(options.binaryCachePolicy == general_bincachepol_reference);

        BOOST_TEST(options.ui_bling == general_ui_bling_reference);

    }

    BOOST_AUTO_TEST_CASE(default_title_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        f::GeneralOptions options;

        // TEST

        YAML::Node node = YAML::Load(yaml_general_notitle);

        f::parse_general_section(options, node);


        BOOST_TEST(options.run_title == general_title_default);

    }

    BOOST_AUTO_TEST_CASE(default_loglevel_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        f::GeneralOptions options;

        // TEST

        YAML::Node node = YAML::Load(yaml_general_nologlevel);

        f::parse_general_section(options, node);


        BOOST_TEST(options.loglevel == general_loglevel_default);

    }

    BOOST_AUTO_TEST_CASE(default_bincachepol_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        f::GeneralOptions options;

        // TEST

        YAML::Node node = YAML::Load(yaml_general_nobincachepol);

        f::parse_general_section(options, node);


        BOOST_TEST(options.binaryCachePolicy == general_bincachepol_default);

    }

    BOOST_AUTO_TEST_CASE(default_ui_bling_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        f::GeneralOptions options;

        // TEST

        YAML::Node node = YAML::Load(yaml_general_nouibling);

        f::parse_general_section(options, node);


        BOOST_TEST(options.ui_bling == general_uibling_default);

    }


BOOST_AUTO_TEST_SUITE_END();