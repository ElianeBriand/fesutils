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

#include <fmt/format.h>


#include <boost/test/unit_test.hpp>

#include "../../file_reader/parse_space_separated_values_node.hpp"

namespace f = fesutils;

BOOST_AUTO_TEST_SUITE(parse_space_separated_values_ts)

    BOOST_AUTO_TEST_CASE(parse_space_separated_double_node_tc, *boost::unit_test::tolerance(0.001)) {
        f::parse_space_separated_double_node node(4);

        std::vector<std::string> example = {"1.0  2.0 -1.2 0.0",
                                            "0.0 inf 8.4e10 12",
                                            "-1 3.33 4.0 nan"};
        auto example_sptr = std::make_shared<std::vector<std::string>>(example);

        auto res = node(example_sptr);

        BOOST_REQUIRE(res->rows() == 3);
        BOOST_REQUIRE(res->cols() == 4);

        BOOST_TEST((*res)(0,0) == 1.0);
        BOOST_TEST((*res)(0,1) == 2.0);
        BOOST_TEST((*res)(0,2) == -1.2);
        BOOST_TEST((*res)(0,3) == 0.0);
        BOOST_TEST((*res)(1,0) == 0.0);
        BOOST_TEST((*res)(1,1) == std::numeric_limits<double>::infinity());
        BOOST_TEST((*res)(1,2) == 8.4e10);
        BOOST_TEST((*res)(1,3) == 12);
        BOOST_TEST((*res)(2,0) == -1);
        BOOST_TEST((*res)(2,1) == 3.33);
        BOOST_TEST((*res)(2,2) == 4.0);
        BOOST_TEST((*res)(2,3) != (*res)(2,3));
    }

    BOOST_AUTO_TEST_CASE(parse_space_separated_double_node_lotsofdata_tc, *boost::unit_test::tolerance(0.001)) {
        f::parse_space_separated_double_node node(4);

        size_t num_val = 100;

        std::vector<std::string> example;
        std::vector<double> val_ones;
        val_ones.reserve(num_val);
        std::vector<double> val_twos;
        val_twos.reserve(num_val);
        for(int i = 0; i < num_val; i++) {
            const double val1 = double(i)/10.0;
            const double val2 = double(i)*125.0 + val1/10.0;
            val_ones.push_back(val1);
            val_twos.push_back(val2);
            std::string s = fmt::format("{} {} {} {}\n", val1, val2, val2, val1);
            example.push_back(s);
        }

        auto example_sptr = std::make_shared<std::vector<std::string>>(example);


        std::shared_ptr<Eigen::Array<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor>> res = node(example_sptr);

        BOOST_REQUIRE(res->rows() == num_val);
        BOOST_REQUIRE(res->cols() == 4);

        for(int i = 0; i < num_val; i++) {
            BOOST_TEST((*res)(i, 0) == val_ones[i]);
            BOOST_TEST((*res)(i, 3) == val_ones[i]);
            BOOST_TEST((*res)(i, 1) == val_twos[i]);
            BOOST_TEST((*res)(i, 2) == val_twos[i]);
            if(     !((*res)(i, 0) == val_ones[i]) ||
                    !((*res)(i, 3) == val_ones[i]) ||
                    !((*res)(i, 1) == val_twos[i]) ||
                    !((*res)(i, 2) == val_twos[i])
            ) {
                fmt::print("i = {} res =[ {} {} {} {}] val_1 = {} val_2 = {}\n", i,
                        (*res)(i, 0), (*res)(i, 1), (*res)(i, 2), (*res)(i, 3),
                            val_ones[i], val_twos[i]);
            }
            BOOST_REQUIRE((*res)(i, 0) == val_ones[i]);
            BOOST_REQUIRE((*res)(i, 3) == val_ones[i]);
            BOOST_REQUIRE((*res)(i, 1) == val_twos[i]);
            BOOST_REQUIRE((*res)(i, 2) == val_twos[i]);
        }
    }


    BOOST_AUTO_TEST_CASE(parse_space_separated_double_node_incomplete_line_tc, *boost::unit_test::tolerance(0.001)) {
        f::parse_space_separated_double_node node(4);

        std::vector<std::string> example = {"1.0  2.0 -1.2 0.0",
                                            "0.0 inf 8.4e10",
                                            "-1 3.33 4.0 nan"};
        auto example_sptr = std::make_shared<std::vector<std::string>>(example);

        auto res = node(example_sptr);

        BOOST_REQUIRE(res->rows() == 2);
        BOOST_REQUIRE(res->cols() == 4);

        BOOST_TEST((*res)(0,0) == 1.0);
        BOOST_TEST((*res)(0,1) == 2.0);
        BOOST_TEST((*res)(0,2) == -1.2);
        BOOST_TEST((*res)(0,3) == 0.0);
        BOOST_TEST((*res)(1,0) == -1);
        BOOST_TEST((*res)(1,1) == 3.33);
        BOOST_TEST((*res)(1,2) == 4.0);
        BOOST_TEST((*res)(1,3) != (*res)(1,3));

    }

BOOST_AUTO_TEST_SUITE_END();