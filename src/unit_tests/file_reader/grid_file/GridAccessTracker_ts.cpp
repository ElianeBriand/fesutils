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


#include "../../../file_reader/grid_file/GridData.hpp"
#include "../../../file_reader/grid_file/GridAccessTracker.hpp"

namespace f = fesutils;

BOOST_AUTO_TEST_SUITE(grid_access_tracker_ts)

    BOOST_AUTO_TEST_CASE(write_access_tracker_tc, *boost::unit_test::tolerance(0.001)) {

        // PRELUDE
        const unsigned int num_axis = 2;
        std::vector<unsigned int> bins_per_axis = {5+1, 2+1};
        std::vector<double> min_bin_value_per_axis = {0.0, 2.0};
        std::vector<double> max_bin_value_per_axis = {0.5, 4.0};
        bool track_access_number = true;

        std::shared_ptr<f::GridData> gd = f::GridData_factory(f::GridData_storage_class::inmemory,
                                                              num_axis,
                                                              bins_per_axis,
                                                              min_bin_value_per_axis,
                                                              max_bin_value_per_axis,
                                                              track_access_number);

        BOOST_TEST(gd->get_num_axis() == 2);
        BOOST_REQUIRE(gd->get_num_axis() == 2);
        BOOST_TEST(gd->get_num_voxels() == (5+1)*(2+1));
        BOOST_TEST(gd->get_bins_per_axis() == bins_per_axis);
        BOOST_TEST(gd->get_min_bin_value_per_axis() == min_bin_value_per_axis);
        BOOST_TEST(gd->get_max_bin_value_per_axis() == max_bin_value_per_axis);
        BOOST_TEST(gd->has_small_bin_width() == false);

        std::vector<double> expected_width = {0.1, 1.0};
        std::vector<double> actual_width = gd->get_bin_width_per_axis();
        for(int axis = 0; axis < num_axis; axis++) {
            BOOST_TEST_INFO(fmt::format("axis = {}",axis));
            BOOST_TEST(expected_width[axis] == actual_width[axis]);
        }

        std::vector<std::vector<double>> expected_edges = {{-0.05, 0.05, 0.15, 0.25, 0.35, 0.45, 0.55  },
                                                           {1.5, 2.5, 3.5, 4.5}};
        std::vector<std::vector<double>> actual_edges = gd->get_bin_edges_per_axis();
        for(int axis = 0; axis < num_axis; axis++) {
            BOOST_TEST(expected_edges[axis].size() ==  actual_edges[axis].size());
            BOOST_REQUIRE(expected_edges[axis].size() ==  actual_edges[axis].size());
            for(int i =0; i < expected_edges[axis].size(); i++) {
                BOOST_TEST_INFO(fmt::format("axis = {} | index = {}",axis, i));
                BOOST_TEST(expected_edges[axis][i] == actual_edges[axis][i]);
            }
        }

        // ACTUAL TEST

        std::vector<double> coord_buffer(2, 0.0);
        std::vector<long int> idx_buffer(2, 0);


        coord_buffer = {0.2, 2.4}; // 3rd bin, 1st bin -> index 2, 0
        idx_buffer = {0, 0};
        bool in_range = gd->insert_at_coord_rangechecked(coord_buffer, 0.6, idx_buffer);
        BOOST_TEST(in_range == true);
        BOOST_REQUIRE(in_range == true);

        bool no_or_one_write_access = gd->parallel_grid_write_posthoc_check();

        BOOST_TEST(no_or_one_write_access == true);

        // Try out of range insert then...

        coord_buffer = {0.1, 10.0};
        idx_buffer = {0, 0};
        in_range = gd->insert_at_coord_rangechecked(coord_buffer, 0.4, idx_buffer);
        BOOST_TEST(in_range == false);
        no_or_one_write_access = gd->parallel_grid_write_posthoc_check();

        BOOST_TEST(no_or_one_write_access == true); // unchanged


        // Insert value in same bin
        coord_buffer = {0.2, 2.4}; // 3rd bin, 1st bin -> index 2, 0
        idx_buffer = {0, 0};
        in_range = gd->insert_at_coord_rangechecked(coord_buffer, 0.8, idx_buffer);
        BOOST_TEST(in_range == true);
        BOOST_REQUIRE(in_range == true);

        no_or_one_write_access = gd->parallel_grid_write_posthoc_check();

        BOOST_TEST(no_or_one_write_access == false);


    }

BOOST_AUTO_TEST_SUITE_END();