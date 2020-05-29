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

#include "../../grid/ingest_grid.hpp"
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

        /*
         * std::shared_ptr<Grid> ingest_fes_grid(const GeneralOptions& options,
                                          const std::string& grid_path,
                                          PlumedDatHeader& header_grid,
                                          const std::optional<std::string>& override_freenergy_field_name  = std::optional<std::string>())
         */
        f::GeneralOptions generalOptions;

        {
            f::PlumedDatHeader header = f::read_cv_file_header(tempFile.getName());


            std::shared_ptr<f::Grid> grid = f::ingest_fes_grid(generalOptions, tempFile.getName(), header);

            BOOST_LOG_TRIVIAL(debug) << header;



            BOOST_TEST(grid->num_dims == 1);
            BOOST_REQUIRE(grid->dims.size() == 1);
            BOOST_TEST(grid->dims[0] == 3);

            BOOST_REQUIRE(grid->min_vals.size() == 1);
            BOOST_TEST(grid->min_vals[0] == 0.1);

            BOOST_REQUIRE(grid->max_vals.size() == 1);
            BOOST_TEST(grid->max_vals[0] == 0.7);

            BOOST_REQUIRE(grid->bin_sizes.size() == 1);
            BOOST_TEST(grid->bin_sizes[0] == 0.2);

            BOOST_REQUIRE(grid->bin_edges.size() == 1);
            BOOST_TEST(grid->bin_edges[0][0] == 0.1);

            std::vector<double> coord = {0.1};
            double val = grid->get_inrange(coord);
            BOOST_TEST(val == 1.0);

            coord = {0.2};
            val = grid->get_inrange(coord);
            BOOST_TEST(val == 1.0);

            coord = {0.358};
            val = grid->get_inrange(coord);
            BOOST_TEST(val == 2.0);

            coord = {0.55};
            val = grid->get_inrange(coord);
            BOOST_TEST(val == 4.0);

            coord = {0.49};
            val = grid->get_inrange(coord);
            BOOST_TEST(val == 2.0);
        }

        {
            f::PlumedDatHeader header = f::read_cv_file_header(tempFile.getName());


            std::shared_ptr<f::Grid> grid = f::ingest_fes_grid(generalOptions, tempFile.getName(), header, "unrelated");

            BOOST_LOG_TRIVIAL(debug) << header;



            BOOST_TEST(grid->num_dims == 1);
            BOOST_REQUIRE(grid->dims.size() == 1);
            BOOST_TEST(grid->dims[0] == 3);

            BOOST_REQUIRE(grid->min_vals.size() == 1);
            BOOST_TEST(grid->min_vals[0] == 0.1);

            BOOST_REQUIRE(grid->max_vals.size() == 1);
            BOOST_TEST(grid->max_vals[0] == 0.7);

            BOOST_REQUIRE(grid->bin_sizes.size() == 1);
            BOOST_TEST(grid->bin_sizes[0] == 0.2);

            BOOST_REQUIRE(grid->bin_edges.size() == 1);
            BOOST_TEST(grid->bin_edges[0][0] == 0.1);

            std::vector<double> coord = {0.1};
            double val = grid->get_inrange(coord);
            BOOST_TEST(val == 8.2);

            coord = {0.2};
            val = grid->get_inrange(coord);
            BOOST_TEST(val == 8.2);

            coord = {0.358};
            val = grid->get_inrange(coord);
            BOOST_TEST(val == 9.2);

            coord = {0.55};
            val = grid->get_inrange(coord);
            BOOST_TEST(val == 11.2);

            coord = {0.49};
            val = grid->get_inrange(coord);
            BOOST_TEST(val == 9.2);
        }



    }

    BOOST_AUTO_TEST_CASE(ingest_grid_no_freenergy_tc, *boost::unit_test::tolerance(0.001)) {
        const std::string filecontent = "#! FIELDS aaa pos unrelated \n"
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

        /*
         * std::shared_ptr<Grid> ingest_fes_grid(const GeneralOptions& options,
                                          const std::string& grid_path,
                                          PlumedDatHeader& header_grid,
                                          const std::optional<std::string>& override_freenergy_field_name  = std::optional<std::string>())
         */
        f::GeneralOptions generalOptions;

        f::PlumedDatHeader header = f::read_cv_file_header(tempFile.getName());


        BOOST_CHECK_THROW(std::shared_ptr<f::Grid> grid = f::ingest_fes_grid(generalOptions, tempFile.getName(), header),
                std::runtime_error);



    }


BOOST_AUTO_TEST_SUITE_END();