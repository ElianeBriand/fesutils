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
#include "../../progress/progress_printer_nodes.hpp"

namespace f = fesutils;
namespace tt = boost::test_tools;


BOOST_AUTO_TEST_SUITE(progress_printer_ts)

    BOOST_AUTO_TEST_CASE(progress_printer_total_tc) {
        f::Progress_printer p;

        BOOST_TEST(p.get_work_packet_total_count() == 0);

        p.increment_workpacket_total_count();

        BOOST_TEST(p.get_work_packet_total_count() == 1);

        p.finish();
    }

    BOOST_AUTO_TEST_CASE(progress_printer_done_tc) {
        f::Progress_printer p;

        BOOST_TEST(p.get_work_packet_done_count() == 0);

        p.increment_workpacket_done_count();

        BOOST_TEST(p.get_work_packet_done_count() == 1);

        p.finish();
    }

    BOOST_AUTO_TEST_CASE(progress_printer_exercice_printing_tc) {
        f::Progress_printer p;

        BOOST_TEST(p.get_work_packet_done_count() == 0);
        BOOST_TEST(p.get_work_packet_total_count() == 0);

        p.increment_workpacket_total_count();
        p.increment_workpacket_total_count();
        p.increment_workpacket_total_count();
        p.increment_workpacket_total_count();
        std::this_thread::sleep_for(std::chrono::milliseconds(25));

        BOOST_TEST(p.get_work_packet_done_count() == 0);
        BOOST_TEST(p.get_work_packet_total_count() == 4);

        p.increment_workpacket_done_count();
        p.increment_workpacket_done_count();
        std::this_thread::sleep_for(std::chrono::milliseconds(25));

        BOOST_TEST(p.get_work_packet_done_count() == 2);
        BOOST_TEST(p.get_work_packet_total_count() == 4);

        p.finish();
    }

    BOOST_AUTO_TEST_CASE(progress_printer_node_wp__tc) {
        f::Progress_printer p;

        BOOST_TEST(p.get_work_packet_done_count() == 0);
        BOOST_TEST(p.get_work_packet_total_count() == 0);

        f::progress_work_packet_registerer<tbb::flow::continue_msg> reg_node(p);

        f::progress_work_packet_done_reporter<tbb::flow::continue_msg> done_node(p);

        reg_node(tbb::flow::continue_msg());

        BOOST_TEST(p.get_work_packet_done_count() == 0);
        BOOST_TEST(p.get_work_packet_total_count() == 1);

        done_node(tbb::flow::continue_msg());

        BOOST_TEST(p.get_work_packet_done_count() == 1);
        BOOST_TEST(p.get_work_packet_total_count() == 1);

        p.finish();
    }

BOOST_AUTO_TEST_SUITE_END();