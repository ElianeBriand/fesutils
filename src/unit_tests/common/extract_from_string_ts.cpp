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

#include "../../common/extract_from_string.hpp"

namespace f = fesutils;

BOOST_AUTO_TEST_SUITE(extract_from_string_ts)

    BOOST_AUTO_TEST_CASE(extract_range_from_string_general_tc, *boost::unit_test::tolerance(0.001)) {
        {
            std::vector<float> res = f::extract_range_from_string("1.0,2.0,3.0");


            BOOST_REQUIRE(res.size() == 3);
            BOOST_TEST(res[0] == 1.0);
            BOOST_TEST(res[1] == 2.0);
            BOOST_TEST(res[2] == 3.0);
        }

        {
            std::vector<float> res = f::extract_range_from_string(" 1.0, 2.0,  3.0   ");

            BOOST_REQUIRE(res.size() == 3);
            BOOST_TEST(res[0] == 1.0);
            BOOST_TEST(res[1] == 2.0);
            BOOST_TEST(res[2] == 3.0);
        }

        {
            std::vector<float> res = f::extract_range_from_string("-1.0, 2.0,  -3.0   ");

            BOOST_REQUIRE(res.size() == 3);
            BOOST_TEST(res[0] == -1.0);
            BOOST_TEST(res[1] == 2.0);
            BOOST_TEST(res[2] == -3.0);
        }

        {
            std::vector<float> res = f::extract_range_from_string(" 1.0e-4, 2.0,  3.0   ");

            BOOST_REQUIRE(res.size() == 3);
            BOOST_TEST(res[0] == 1.0e-4);
            BOOST_TEST(res[1] == 2.0);
            BOOST_TEST(res[2] == 3.0);
        }

        {
            std::vector<float> res = f::extract_range_from_string(" 1.0");

            BOOST_REQUIRE(res.size() == 1);
            BOOST_TEST(res[0] == 1.0);
        }
        {
            std::vector<float> res = f::extract_range_from_string("");

            BOOST_REQUIRE(res.size() == 0);
        }

        {
            std::vector<float> res = f::extract_range_from_string("fgdseezs");
            BOOST_REQUIRE(res.size() == 0);
        }


    }

    BOOST_AUTO_TEST_CASE(slow_tokenize_tc) {
        {
            std::vector<std::string> res = f::slow_tokenize("a bbb ABC");

            BOOST_REQUIRE(res.size() == 3);
            BOOST_TEST(res[0] == "a");
            BOOST_TEST(res[1] == "bbb");
            BOOST_TEST(res[2] == "ABC");
        }

        {
            std::vector<std::string> res = f::slow_tokenize("");

            BOOST_REQUIRE(res.size() == 0);
        }

        {
            std::vector<std::string> res = f::slow_tokenize("ABC");

            BOOST_REQUIRE(res.size() == 1);
            BOOST_TEST(res[0] == "ABC");
        }

        {
            std::vector<std::string> res = f::slow_tokenize("ABC   ");

            BOOST_REQUIRE(res.size() == 1);
            BOOST_TEST(res[0] == "ABC");
        }

    }

    BOOST_AUTO_TEST_CASE(parse_bool_as_word_tc) {
        bool res = f::parse_bool_as_word("true");
        BOOST_TEST(res);

        res = f::parse_bool_as_word("false");
        BOOST_TEST(!res);

        BOOST_CHECK_THROW( f::parse_bool_as_word("fhf"), std::runtime_error );
        BOOST_CHECK_THROW( f::parse_bool_as_word("false "), std::runtime_error );


    }

    BOOST_AUTO_TEST_SUITE_END();