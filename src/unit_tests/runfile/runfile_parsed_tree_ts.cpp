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


namespace f = fesutils;

BOOST_TEST_DONT_PRINT_LOG_VALUE(f::StepFunction);

BOOST_AUTO_TEST_SUITE(runfile_parsed_tree_ts)

    BOOST_AUTO_TEST_CASE(StepFunction_conversion_tc, *boost::unit_test::tolerance(0.001)) {

        f::StepFunction s = f::string_to_stepfunction("fes_data");
        BOOST_TEST(s == f::StepFunction::fes_data);

        s = f::string_to_stepfunction("reweight");
        BOOST_TEST(s == f::StepFunction::reweight);

        s = f::string_to_stepfunction("histogram");
        BOOST_TEST(s == f::StepFunction::histogram);

        s = f::string_to_stepfunction("reconstruction");
        BOOST_TEST(s == f::StepFunction::reconstruction);

        BOOST_CHECK_THROW(f::string_to_stepfunction("nonexistent"), std::runtime_error);

        BOOST_TEST("fes_data" == stepfunction_to_string(f::StepFunction::fes_data));
        BOOST_TEST("reweight" == stepfunction_to_string(f::StepFunction::reweight));
        BOOST_TEST("histogram" == stepfunction_to_string(f::StepFunction::histogram));
        BOOST_TEST("reconstruction" == stepfunction_to_string(f::StepFunction::reconstruction));


    }


BOOST_AUTO_TEST_SUITE_END();