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
#include "../../runfile/runfile_steps.hpp"


namespace f = fesutils;

namespace {
    const std::string yaml_steps_correct = "    - fes_data : [bzn]\n"
                                           "    - reweight: rw_all\n";

    const std::string yaml_steps_notasequence = "    fes_data : [bzn]\n"
                                           "    reweight: rw_all\n";

    const std::string yaml_steps_notasequenceofmap = "    - [bzn]\n"
                                           "    - rw_all\n";

    const std::string yaml_steps_incorrect_struct_misc = "    - fes_data : {a: b}\n"
                                           "    - reweight: rw_all\n";
}

BOOST_TEST_DONT_PRINT_LOG_VALUE(f::StepFunction);

BOOST_AUTO_TEST_SUITE(runfile_steps_ts)

    BOOST_AUTO_TEST_CASE(steps_correct_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        f::GeneralOptions options;

        // Dummy pre-registered steps
        f::g_runfileitem_fesdata_list["bzn"] = f::RunfileItem_FESData();
        f::g_runfileitem_reweight_list["rw_all"] = f::RunfileItem_Reweight();

        // TEST

        YAML::Node node = YAML::Load(yaml_steps_correct);

        f::parse_steps_section(options, node);

        BOOST_TEST(f::g_all_steps_list.size() == 2);
        BOOST_REQUIRE(f::g_all_steps_list.size() == 2);
        BOOST_TEST(f::g_remaining_steps_lists.size() == 2);
        BOOST_REQUIRE(f::g_remaining_steps_lists.size() == 2);

        BOOST_TEST(f::g_all_steps_list.at(0).function == f::StepFunction::fes_data);
        BOOST_TEST(f::g_all_steps_list.at(1).function == f::StepFunction::reweight);

        BOOST_TEST(f::g_all_steps_list.at(0).parameter_set_names == std::vector<std::string>({"bzn"}));
        BOOST_TEST(f::g_all_steps_list.at(1).parameter_set_names == std::vector<std::string>({"rw_all"}));

        f::g_all_steps_list.clear();
        f::g_runfileitem_fesdata_list.clear();
        f::g_runfileitem_reweight_list.clear();
    }

    BOOST_AUTO_TEST_CASE(steps_not_correct_structure_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        f::GeneralOptions options;

        // Dummy pre-registered steps
        f::g_runfileitem_fesdata_list["bzn"] = f::RunfileItem_FESData();
        f::g_runfileitem_reweight_list["rw_all"] = f::RunfileItem_Reweight();

        // TEST

        YAML::Node node = YAML::Load(yaml_steps_notasequence);

        BOOST_CHECK_THROW(f::parse_steps_section(options, node), std::runtime_error);

        node = YAML::Load(yaml_steps_notasequenceofmap);

        BOOST_CHECK_THROW(f::parse_steps_section(options, node), std::runtime_error);

        node = YAML::Load(yaml_steps_incorrect_struct_misc);

        BOOST_CHECK_THROW(f::parse_steps_section(options, node), std::runtime_error);

        f::g_all_steps_list.clear();
        f::g_runfileitem_fesdata_list.clear();
        f::g_runfileitem_reweight_list.clear();

    }

    BOOST_AUTO_TEST_CASE(missing_param_set_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        f::GeneralOptions options;

        // Dummy pre-registered steps, missing one
        f::g_runfileitem_fesdata_list["bzn"] = f::RunfileItem_FESData();

        // TEST

        YAML::Node node = YAML::Load(yaml_steps_correct);

        BOOST_CHECK_THROW(f::parse_steps_section(options, node), std::runtime_error);

        f::g_all_steps_list.clear();
        f::g_runfileitem_fesdata_list.clear();
        f::g_runfileitem_reweight_list.clear();
    }




BOOST_AUTO_TEST_SUITE_END();