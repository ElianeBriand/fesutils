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
#include "../../common/NotImplementedError.hpp"


BOOST_AUTO_TEST_SUITE(not_implemented_error_ts)

    BOOST_AUTO_TEST_CASE(ctor_tc, *boost::unit_test::tolerance(0.001)) {
        not_implemented_error a("aaa");

        BOOST_TEST(a.what() == "aaa");

        not_implemented_error b(std::string("aaa"));

        BOOST_TEST(b.what() == "aaa");

        // Dtor explicit usage for coverage purpose
        not_implemented_error* c = new not_implemented_error(std::string("aaa"));
        delete c;

    }


BOOST_AUTO_TEST_SUITE_END();