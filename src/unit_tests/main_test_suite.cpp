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


BOOST_AUTO_TEST_SUITE(misc_ts)


    BOOST_AUTO_TEST_CASE(empty_example_tc,* boost::unit_test::timeout(10)) {
        BOOST_TEST(true == true);
    }

BOOST_AUTO_TEST_SUITE_END();