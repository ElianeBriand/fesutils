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

    BOOST_AUTO_TEST_CASE(factory_tc, *boost::unit_test::tolerance(0.01)) {

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

        std::vector<std::vector<double>> expected_centers = {{0.0, 0.1, 0.2, 0.3, 0.4, 0.5},
                                                           {2.0, 3.0, 4.0}};
        std::vector<std::vector<double>> actual_centers = gd->get_bin_centers_per_axis();
        for(int axis = 0; axis < num_axis; axis++) {
            BOOST_TEST(expected_centers[axis].size() ==  actual_centers[axis].size());
            BOOST_REQUIRE(expected_centers[axis].size() ==  actual_centers[axis].size());
            for(int i =0; i < expected_centers[axis].size(); i++) {
                BOOST_TEST_INFO(fmt::format("axis = {} | index = {}",axis, i));
                BOOST_TEST(expected_centers[axis][i] == actual_centers[axis][i]);
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

    BOOST_AUTO_TEST_CASE(insert_into_grid_tc, *boost::unit_test::tolerance(0.001)) {

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

        // Some out of range insert first...

        coord_buffer = {0.1, 5.0};
        bool out_of_range = gd->insert_at_coord_rangechecked(coord_buffer, 0.4, idx_buffer);
        BOOST_TEST(out_of_range == false);

        coord_buffer = {0.9, 5.0};
        out_of_range = gd->insert_at_coord_rangechecked(coord_buffer, 0.4, idx_buffer);
        BOOST_TEST(out_of_range == false);

        coord_buffer = {-0.9, 5.0};
        out_of_range = gd->insert_at_coord_rangechecked(coord_buffer, 0.4, idx_buffer);
        BOOST_TEST(out_of_range == false);

        coord_buffer = {-0.9, 3.0};
        out_of_range = gd->insert_at_coord_rangechecked(coord_buffer, 0.4, idx_buffer);
        BOOST_TEST(out_of_range == false);

        // Then check for coord indices generation in range...

        coord_buffer = {0.2, 2.4}; // 3rd bin, 1st bin -> index 2, 0
        bool in_range_idx = gd->coord_to_indices_rangechecked(coord_buffer, idx_buffer);
        BOOST_TEST(in_range_idx == true);
        BOOST_TEST(idx_buffer[0] == 2);
        BOOST_TEST(idx_buffer[1] == 0);

        // Check the initial value of zero...

        idx_buffer = {0, 0};
        double value = 1636.4;
        gd->get_value_at_coord_rangechecked(coord_buffer, value, idx_buffer);
        BOOST_TEST(value == 0.0);

        // Insert and check for value...

        idx_buffer = {0, 0};
        bool in_range = gd->insert_at_coord_rangechecked(coord_buffer, 0.4, idx_buffer);
        BOOST_TEST(in_range == true);

        idx_buffer = {0, 0};
        value = 1636.4;
        in_range = gd->get_value_at_coord_rangechecked(coord_buffer, value, idx_buffer);
        BOOST_TEST(in_range == true);
        BOOST_TEST(value == 0.4);

        // Then some out of range read...

        coord_buffer = {-0.9, 3.0}; // out of range
        idx_buffer = {0, 0};
        value = 1636.4;
        in_range = gd->get_value_at_coord_rangechecked(coord_buffer, value, idx_buffer);
        BOOST_TEST(in_range == false);
        BOOST_TEST(value == 1636.4);


    }

    BOOST_AUTO_TEST_CASE(border_case_tc, *boost::unit_test::tolerance(0.001)) {

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


        // Then check for coord indices generation when at the limites of the ranges

        coord_buffer = {0.0, 4.0}; // 1st bin, 3rd bin -> index 0, 2
        bool in_range_idx = gd->coord_to_indices_rangechecked(coord_buffer, idx_buffer);
        BOOST_TEST(in_range_idx == true);
        BOOST_TEST(idx_buffer[0] == 0);
        BOOST_TEST(idx_buffer[1] == 2);

        coord_buffer = {-0.1, 4.0}; // 1st bin, 3rd bin -> index 0, 1
        bool out_of_range_idx = gd->coord_to_indices_rangechecked(coord_buffer, idx_buffer);
        BOOST_TEST(out_of_range_idx == false);


        // Check all four limit corner
        coord_buffer = {0.0, 2.0};
        idx_buffer = {0, 0};
        double value = 1636.4;
        double corner = 1.0;
        idx_buffer = {0, 0};
        bool in_range = gd->insert_at_coord_rangechecked(coord_buffer, 0.4 + corner, idx_buffer);
        BOOST_TEST(in_range == true);
        idx_buffer = {0, 0};
        value = 1636.4;
        in_range = gd->get_value_at_coord_rangechecked(coord_buffer, value, idx_buffer);
        BOOST_TEST(in_range == true);
        BOOST_TEST(value == 0.4 + corner);

        coord_buffer = {0.5, 4.0};
        idx_buffer = {0, 0};
        value = 1636.4;
        corner = 2.0;
        idx_buffer = {0, 0};
        in_range = gd->insert_at_coord_rangechecked(coord_buffer, 0.4 + corner, idx_buffer);
        BOOST_TEST(in_range == true);
        idx_buffer = {0, 0};
        value = 1636.4;
        in_range = gd->get_value_at_coord_rangechecked(coord_buffer, value, idx_buffer);
        BOOST_TEST(in_range == true);
        BOOST_TEST(value == 0.4 + corner);

        coord_buffer = {0.0, 4.0};
        idx_buffer = {0, 0};
        value = 1636.4;
        corner = 3.0;
        idx_buffer = {0, 0};
        in_range = gd->insert_at_coord_rangechecked(coord_buffer, 0.4 + corner, idx_buffer);
        BOOST_TEST(in_range == true);
        idx_buffer = {0, 0};
        value = 1636.4;
        in_range = gd->get_value_at_coord_rangechecked(coord_buffer, value, idx_buffer);
        BOOST_TEST(in_range == true);
        BOOST_TEST(value == 0.4 + corner);

        coord_buffer = {0.5, 2.0};
        idx_buffer = {0, 0};
        corner = 4.0;
        idx_buffer = {0, 0};
        in_range = gd->insert_at_coord_rangechecked(coord_buffer, 0.4 + corner, idx_buffer);
        BOOST_TEST(in_range == true);
        idx_buffer = {0, 0};
        value = 1636.4;
        in_range = gd->get_value_at_coord_rangechecked(coord_buffer, value, idx_buffer);
        BOOST_TEST(in_range == true);
        BOOST_TEST(value == 0.4 + corner);

        // Check for borderline values in the table
        // First insert the values in the relevant bins
        coord_buffer = {0.1, 3.0};
        idx_buffer = {0, 0};
        value = 4.25;
        in_range = gd->insert_at_coord_rangechecked(coord_buffer, value, idx_buffer);
        BOOST_TEST(in_range == true);
        BOOST_TEST(idx_buffer[0] == 1);
        BOOST_TEST(idx_buffer[1] == 1);

        // Check read back

        idx_buffer = {0, 0};
        value = 1636.4;
        in_range = gd->get_value_at_coord_rangechecked(coord_buffer, value, idx_buffer);
        BOOST_TEST(in_range == true);
        BOOST_TEST(value == 4.25);
        BOOST_TEST(idx_buffer[0] == 1);
        BOOST_TEST(idx_buffer[1] == 1);


        // Check at boundary values
        /*        std::vector<std::vector<double>> expected_edges = {{-0.05, 0.05, 0.15, 0.25, 0.35, 0.45, 0.55  },
                                                           {1.5, 2.5, 3.5, 4.5}};*/
        coord_buffer = {0.051, 2.6};
        idx_buffer = {0, 0};
        value = 1636.4;
        in_range = gd->get_value_at_coord_rangechecked(coord_buffer, value, idx_buffer);
        BOOST_TEST(in_range == true);
        BOOST_TEST(value == 4.25);
        BOOST_TEST(idx_buffer[0] == 1);
        BOOST_TEST(idx_buffer[1] == 1);

        coord_buffer = {0.149, 2.6};
        idx_buffer = {0, 0};
        value = 1636.4;
        in_range = gd->get_value_at_coord_rangechecked(coord_buffer, value, idx_buffer);
        BOOST_TEST(in_range == true);
        BOOST_TEST(value == 4.25);
        BOOST_TEST(idx_buffer[0] == 1);
        BOOST_TEST(idx_buffer[1] == 1);

        coord_buffer = {0.151, 3.4};
        idx_buffer = {0, 0};
        value = 1636.4;
        in_range = gd->get_value_at_coord_rangechecked(coord_buffer, value, idx_buffer);
        BOOST_TEST(in_range == true);
        BOOST_TEST(value != 4.25);
        BOOST_TEST(idx_buffer[0] == 2);
        BOOST_TEST(idx_buffer[1] == 1);

        coord_buffer = {0.049, 3.4};
        idx_buffer = {0, 0};
        value = 1636.4;
        in_range = gd->get_value_at_coord_rangechecked(coord_buffer, value, idx_buffer);
        BOOST_TEST(in_range == true);
        BOOST_TEST(value != 4.25);
        BOOST_TEST(idx_buffer[0] == 0);
        BOOST_TEST(idx_buffer[1] == 1);

    }

BOOST_AUTO_TEST_SUITE_END();