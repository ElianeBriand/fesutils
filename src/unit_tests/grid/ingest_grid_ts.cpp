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

#include "../MiscFixture.hpp"
#include "../TempTextFile.hpp"
#include "../../file_reader/read_file_header.hpp"

namespace f = fesutils;
namespace tt = boost::test_tools;

#include <boost/log/trivial.hpp>


//, *boost::unit_test::tolerance(0.001)

BOOST_AUTO_TEST_SUITE(ingest_grid_ts)

    BOOST_AUTO_TEST_CASE(ingest_grid_tc, *boost::unit_test::tolerance(0.001)) {
        const std::string filecontent = "#! FIELDS aaa ffpos unrelated \n"
                                        "#! SET normalisation  1.000000\n"
                                        "#! SET min_aaa 0.1\n"
                                        "#! SET max_aaa 0.7\n"
                                        "#! SET nbins_aaa  3\n"
                                        "#! SET periodic_aaa false\n"
                                        "0.1 1.0 8.2\n"
                                        "0.3 2.0 9.2\n"
                                        "0.5 3.0 10.2\n"
                                        "0.7 4.0 11.2\n";

        TempTextFile tempFile(filecontent);




    }

BOOST_AUTO_TEST_SUITE_END();