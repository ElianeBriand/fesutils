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

#include "../frontend_common.h"

namespace f = fesutils;

BOOST_AUTO_TEST_SUITE(frontend_common_ts)

    BOOST_AUTO_TEST_CASE(indentation_level_tc, *boost::unit_test::tolerance(0.001)) {

        std::string prefix_1 = non_boost_log_get_space_prefix();

        BOOST_TEST(prefix_1 == "    ");

        setup_log_formating_increase_indentation();

        std::string prefix_2 = non_boost_log_get_space_prefix();

        BOOST_TEST(prefix_2 == "       ");

        setup_log_formating_decrease_indentation();

        std::string prefix_3 = non_boost_log_get_space_prefix();

        BOOST_TEST(prefix_3 == "    ");

    }


BOOST_AUTO_TEST_SUITE_END();