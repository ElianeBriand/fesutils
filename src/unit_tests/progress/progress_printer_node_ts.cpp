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
#include <tbb/flow_graph.h>

#include "../../progress/progress_printer.hpp"
#include "../../progress/progress_printer_nodes.hpp"

namespace f = fesutils;

BOOST_AUTO_TEST_SUITE(progress_printer_node_ts)

    BOOST_AUTO_TEST_CASE(PerLine_TodoToInflight_node_tc, *boost::unit_test::tolerance(0.001)) {
        f::ProgressPrinter p;

        f::PerLine_TodoToInflight_node node(p);

        p.set_todo_count(100);

        std::shared_ptr<std::vector<std::string>> datapacket = std::make_shared<std::vector<std::string>>(10, "A");

        node(datapacket);

        BOOST_TEST(p.get_todo_count() == 90);
        BOOST_TEST(p.get_inflight_count() == 10);

        node(datapacket);

        BOOST_TEST(p.get_todo_count() == 80);
        BOOST_TEST(p.get_inflight_count() == 20);

        p.finish();

    }

    BOOST_AUTO_TEST_CASE(PerContMsg_InflightToDone_node_tc, *boost::unit_test::tolerance(0.001)) {
        f::ProgressPrinter p;

        f::PerContMsg_InflightToDone_node node(p);

        p.add_to_inflight_count(100);


        node(tbb::flow::continue_msg());

        BOOST_TEST(p.get_done_count() == 1);
        BOOST_TEST(p.get_inflight_count() == 99);

        node(tbb::flow::continue_msg());

        BOOST_TEST(p.get_done_count() == 2);
        BOOST_TEST(p.get_inflight_count() == 98);

        p.finish();
    }

    BOOST_AUTO_TEST_CASE(PerNumItem_InflightToDone_node_tc, *boost::unit_test::tolerance(0.001)) {
        f::ProgressPrinter p;

        f::PerNumItem_InflightToDone_node node(p);

        p.add_to_inflight_count(100);

        node(10);

        BOOST_TEST(p.get_done_count() == 10);
        BOOST_TEST(p.get_inflight_count() == 90);

        node(15);

        BOOST_TEST(p.get_done_count() == 25);
        BOOST_TEST(p.get_inflight_count() == 75);

        p.finish();
    }



BOOST_AUTO_TEST_SUITE_END();