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
#include <stdexcept>
#include <cstdio>
#include <cstdlib>

#include <tbb/flow_graph.h>


#include <boost/test/unit_test.hpp>

#include "../../progress/progress_printer.hpp"


namespace f = fesutils;
namespace tt = boost::test_tools;


BOOST_AUTO_TEST_SUITE(progress_printer_ts)

    BOOST_AUTO_TEST_CASE(progress_printer_total_tc) {
        f::ProgressPrinter p;

        BOOST_TEST(p.get_todo_count() == 0);
        BOOST_TEST(p.get_done_count() == 0);
        BOOST_TEST(p.get_inflight_count() == 0);

        p.add_to_todo_count(120);

        BOOST_TEST(p.get_todo_count() == 120);

        p.substract_from_todo_count(10);

        BOOST_TEST(p.get_todo_count() == 110);

        p.set_todo_count(240);

        BOOST_TEST(p.get_todo_count() == 240);

        p.add_to_inflight_count(20);

        BOOST_TEST(p.get_inflight_count() == 20);

        p.increment_inflight_count();

        BOOST_TEST(p.get_inflight_count() == 21);

        p.decrement_inflight_count();

        BOOST_TEST(p.get_inflight_count() == 20);

        p.substract_from_inflight_count(4);

        BOOST_TEST(p.get_inflight_count() == 16);

        p.increment_done_count();

        BOOST_TEST(p.get_done_count() == 1);

        p.add_to_done_count(15);

        BOOST_TEST(p.get_done_count() == 16);

        p.substract_from_done_count(3);

        BOOST_TEST(p.get_done_count() == 13);


        p.finish();
    }


BOOST_AUTO_TEST_SUITE_END();