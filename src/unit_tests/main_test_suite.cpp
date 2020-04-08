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


#define BOOST_TEST_MODULE main_test_module
#define BOOST_TEST_MAIN

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "../common/dummy_sink_node.hpp"

namespace f = fesutils;

BOOST_AUTO_TEST_SUITE(misc_ts)




    BOOST_AUTO_TEST_CASE(dummy_sink_node_tc,* boost::unit_test::timeout(10)) {
        f::dummy_sink_node<std::vector<double>> node;
        std::vector<double> v = {0.0, 1.0, 3.0};
        bool res = node(v);
        BOOST_TEST(res);
    }

BOOST_AUTO_TEST_SUITE_END();