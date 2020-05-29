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


#include <boost/test/unit_test.hpp>

#include "../../file_reader/field_function_heuristics.hpp"
#include "../../file_reader/read_file_header.hpp"

namespace f = fesutils;

BOOST_AUTO_TEST_SUITE(field_function_heuristics_ts)

    BOOST_AUTO_TEST_CASE(find_unique_field_tc) {
        {
            f::PlumedDatHeader header;
            f::parse_FIELDS_line(header, "#! FIELDS varb varb.bias ffpos");

            std::optional<size_t> likely_a = f::find_unique_field_name_fragment_index(header, "ff");

            BOOST_TEST(likely_a.has_value());
            BOOST_REQUIRE(likely_a.has_value());
            BOOST_TEST(likely_a.value() == 2);

            std::optional<size_t> likely_nothing = f::find_unique_field_name_fragment_index(header, "notafield");

            BOOST_TEST(!likely_nothing.has_value());

            // not unique -> no results
            likely_nothing = f::find_unique_field_name_fragment_index(header, "varb");

            BOOST_TEST(!likely_nothing.has_value());

        }

    }

    BOOST_AUTO_TEST_CASE(likely_bias_likely_fe_tc) {

        {
            f::PlumedDatHeader header;
            f::parse_FIELDS_line(header, "#! FIELDS a b.bias ffpos");

            std::optional<size_t> likely_bias = f::find_likely_bias_field_index(header);

            BOOST_TEST(likely_bias.has_value());
            BOOST_REQUIRE(likely_bias.has_value());
            BOOST_TEST(likely_bias.value() == 1);

            std::optional<size_t> likely_fe = f::find_likely_fenerg_field_index(header);

            BOOST_TEST(likely_fe.has_value());
            BOOST_REQUIRE(likely_fe.has_value());
            BOOST_TEST(likely_fe.value() == 2);
        }


    }


BOOST_AUTO_TEST_SUITE_END();