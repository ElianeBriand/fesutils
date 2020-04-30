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

#include <iostream>

#include <vector>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>

#include <boost/test/unit_test.hpp>

#include "../../grid/grid_utils.hpp"
#include "../MiscFixture.hpp"
#include "../../grid/Grid.hpp"
#include "../../grid/grid_filler_node.hpp"

namespace f = fesutils;
namespace tt = boost::test_tools;


BOOST_AUTO_TEST_SUITE(grid_filler_node_ts)

    BOOST_AUTO_TEST_CASE(grid_filler_node_tc) {
        MiscFixture fixture;

        std::shared_ptr<f::Grid> g = std::make_shared<f::Grid>(fixture.dummy_header);
        std::cout << "grid num_dims = " << g->num_dims << std::endl;
        std::cout << "Index of coord in header: ";
        for(size_t i = 0; i < g->num_dims; i++) {
            size_t coord_index_in_data = g->header_grid_field_indexes[i];
            std::cout << coord_index_in_data << ", ";
        }
        std::cout << std::endl;

        f::grid_filler_node n(g, fixture.index_bias_field);

        size_t num_row = fixture.example_coordinates_inside_grid.size();
        size_t num_cols = fixture.dummy_header.fields.size();
        std::cout << "num_row = " << num_row << " & num_cols = " << num_cols << " & index_bias_field = " << fixture.index_bias_field << std::endl;
        auto data = std::make_shared<Eigen::Array<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor>>(num_row, num_cols);

        BOOST_TEST(g->num_dims == fixture.index_bias_field);

        for(size_t i_row = 0; i_row < num_row; i_row++) {
            std::vector<double> coords = fixture.example_coordinates_inside_grid.at(i_row);
            double value = fixture.examples_value.at(i_row);
            for(size_t i_col = 0; i_col < coords.size(); i_col++) {
                (*data)(i_row, i_col) = coords[i_col];
            }
            (*data)(i_row, coords.size()) = value;
        }
        std::cout << "array dump begin" << std::endl;
        for(size_t i_row = 0; i_row < num_row; i_row++) {
            for(size_t i_col = 0; i_col < num_cols; i_col++) {
                std::cout << (*data)(i_row, i_col) <<", ";
            }
            std::cout << "\n";
        }
        std::cout << "array dump end" << std::endl;


        n(data);


        for(size_t i_row = 0; i_row < num_row; i_row++) {
            std::vector<double> coords = fixture.example_coordinates_inside_grid.at(i_row);
            std::cout << "At coord: ";
            for(double c: coords) {
                std::cout << c << ", ";
            }
            double ref_value = fixture.examples_value.at(i_row);
            double test_val = g->get_inrange(coords);
            std::cout << " -> value = " << test_val << std::endl;
            BOOST_TEST(test_val == ref_value);
        }

    }

#ifndef NDEBUG
    BOOST_AUTO_TEST_CASE(grid_filler_node_incompat_dims_tc) {

        {
            MiscFixture fixture;

            std::shared_ptr<f::Grid> g = std::make_shared<f::Grid>(fixture.dummy_header);

            f::grid_filler_node n(g, fixture.index_bias_field);

            size_t num_row = fixture.example_coordinates_inside_grid.size();
            // Make the number of column too small by 1
            size_t num_cols = fixture.dummy_header.fields.size() - 1;

            std::cout << "num_row = " << num_row << " & num_cols = " << num_cols << " & index_bias_field = " << fixture.index_bias_field << std::endl;

            auto data = std::make_shared<Eigen::Array<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor>>(num_row, num_cols);

            BOOST_CHECK_THROW( n(data), std::runtime_error );
        }

        {
            MiscFixture fixture;

            std::shared_ptr<f::Grid> g = std::make_shared<f::Grid>(fixture.dummy_header);

            // Make the index bias field out of range
            f::grid_filler_node n(g, fixture.index_bias_field + 1);

            size_t num_row = fixture.example_coordinates_inside_grid.size();
            size_t num_cols = fixture.dummy_header.fields.size();

            std::cout << "num_row = " << num_row << " & num_cols = " << num_cols << " & index_bias_field = " << fixture.index_bias_field << std::endl;

            auto data = std::make_shared<Eigen::Array<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor>>(num_row, num_cols);

            BOOST_CHECK_THROW( n(data), std::runtime_error );
        }

    }
#endif

BOOST_AUTO_TEST_SUITE_END();