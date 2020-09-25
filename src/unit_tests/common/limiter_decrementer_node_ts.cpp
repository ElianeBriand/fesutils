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
#include "../../common/limiter_decrementer_node.hpp"


namespace f = fesutils;

template class f::limiter_decrementer_node<int>;

BOOST_AUTO_TEST_SUITE(limiter_decrementer_ts)

    BOOST_AUTO_TEST_CASE(limiter_decrement_tc, *boost::unit_test::tolerance(0.001)) {
        f::limiter_decrementer_node<int> a;
        a(1);
        BOOST_TEST(true); // Just need for instantiation & operator() to return
    }


BOOST_AUTO_TEST_SUITE_END();