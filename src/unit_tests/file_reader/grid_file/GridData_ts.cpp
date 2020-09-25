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


namespace f = fesutils;

BOOST_AUTO_TEST_SUITE(griddata_ts)

    BOOST_AUTO_TEST_CASE(factory_tc, *boost::unit_test::tolerance(0.001)) {

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


    }

    BOOST_AUTO_TEST_CASE(inconsistent_axis_tc, *boost::unit_test::tolerance(0.001)) {

        const unsigned int num_axis = 2;
        std::vector<unsigned int> bins_per_axis = {10+1, 5+1};
        std::vector<double> min_bin_value_per_axis = {0.0}; // INCORRECT: single value for min value
        std::vector<double> max_bin_value_per_axis = {1.0, 7.0};
        bool track_access_number = true;

        BOOST_CHECK_THROW(f::GridData_factory(f::GridData_storage_class::inmemory,
                                                              num_axis,
                                                              bins_per_axis,
                                                              min_bin_value_per_axis,
                                                              max_bin_value_per_axis,
                                                              track_access_number), std::runtime_error);




    }

    BOOST_AUTO_TEST_CASE(single_bin_tc, *boost::unit_test::tolerance(0.001)) {

        const unsigned int num_axis = 2;
        std::vector<unsigned int> bins_per_axis = {10+1, 1};
        std::vector<double> min_bin_value_per_axis = {0.0, 2.0}; // INCORRECT: single value for min value
        std::vector<double> max_bin_value_per_axis = {1.0, 7.0};
        bool track_access_number = true;

        BOOST_CHECK_THROW(f::GridData_factory(f::GridData_storage_class::inmemory,
                                              num_axis,
                                              bins_per_axis,
                                              min_bin_value_per_axis,
                                              max_bin_value_per_axis,
                                              track_access_number), std::runtime_error);

    }

    BOOST_AUTO_TEST_CASE(small_bin_width_tc, *boost::unit_test::tolerance(0.001)) {

        const unsigned int num_axis = 2;
        std::vector<unsigned int> bins_per_axis = {500+1, 2+1};
        std::vector<double> min_bin_value_per_axis = {0.0, 2.0};
        std::vector<double> max_bin_value_per_axis = {0.005, 4.0};
        bool track_access_number = true;

        std::shared_ptr<f::GridData> gd = f::GridData_factory(f::GridData_storage_class::inmemory,
                                                              num_axis,
                                                              bins_per_axis,
                                                              min_bin_value_per_axis,
                                                              max_bin_value_per_axis,
                                                              track_access_number);

        BOOST_TEST(gd->get_num_axis() == 2);
        BOOST_REQUIRE(gd->get_num_axis() == 2);
        BOOST_TEST(gd->has_small_bin_width() == true);


    }


BOOST_AUTO_TEST_SUITE_END();