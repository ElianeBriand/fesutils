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

#include "../../grid/grid_utils.hpp"
#include "../MiscFixture.hpp"
#include "../../grid/Grid.hpp"


namespace f = fesutils;
namespace tt = boost::test_tools;


BOOST_AUTO_TEST_SUITE(grid_ts)

    BOOST_AUTO_TEST_CASE(grid_ctor_tc) {
        MiscFixture fixture;

        f::Grid g(fixture.dummy_header);

        BOOST_TEST(g.dims == fixture.expected_grid_dims, tt::per_element());
        BOOST_TEST(g.num_dims == fixture.expected_grid_dims.size());
        BOOST_TEST(g.header_grid_field_indexes == fixture.expected_grid_field_indexes, tt::per_element());
        BOOST_TEST(g.min_vals == fixture.expected_grid_min_vals, tt::per_element());
        BOOST_TEST(g.max_vals == fixture.expected_grid_max_vals, tt::per_element());
        BOOST_TEST(g.bin_sizes == fixture.expected_grid_bin_sizes, tt::per_element());

        if(g.bin_edges.size() == fixture.expected_grid_bin_edges.size())
        {
            for(int i=0; i < g.bin_edges.size(); i++)
            {
                BOOST_TEST_CONTEXT("dim " << i)
                {
                    BOOST_TEST(g.bin_edges[i] == fixture.expected_grid_bin_edges[i], tt::per_element());
                }
            }
        }
        else
        {
            BOOST_TEST(g.bin_edges.size() == fixture.expected_grid_bin_edges.size());
        }

        if(g.bin_center.size() == fixture.expected_grid_bin_centers.size())
        {
            for(int i=0; i < g.bin_center.size(); i++)
            {
                BOOST_TEST_CONTEXT("dim " << i)
                {
                    BOOST_TEST(g.bin_center[i] == fixture.expected_grid_bin_centers[i], tt::per_element());
                }
            }
        }
        else
        {
            BOOST_TEST(g.bin_center.size() == fixture.expected_grid_bin_centers.size());
        }

    }

    BOOST_AUTO_TEST_CASE(grid_ctor_no_grid_var_tc) {
        f::PlumedDatHeader header;
        BOOST_CHECK_THROW(f::Grid g(header), std::runtime_error);
    }


    BOOST_AUTO_TEST_CASE(grid_set_get_tc) {
        MiscFixture fixture;

        {
            f::Grid g(fixture.dummy_header);

            // Insert some example data
            std::vector<long int> index_buffer(g.num_dims);
            for(int i = 0; i < fixture.example_coordinates_inside_grid.size(); i++) {
                g.set_inrange(fixture.example_coordinates_inside_grid[i], fixture.examples_value[i],index_buffer);
            }

            for(int i = 0; i < fixture.example_coordinates_inside_grid.size(); i++) {
                BOOST_TEST_CONTEXT("value " << i)
                {
                    BOOST_TEST(g.get_inrange(fixture.example_coordinates_inside_grid[i], index_buffer) == fixture.examples_value[i]);
                }
            }
        }

        {
            f::Grid g(fixture.dummy_header);

            // Insert some example data
            for(int i = 0; i < fixture.example_coordinates_inside_grid.size(); i++) {
                g.set_inrange(fixture.example_coordinates_inside_grid[i], fixture.examples_value[i]);
            }

            for(int i = 0; i < fixture.example_coordinates_inside_grid.size(); i++) {
                BOOST_TEST_CONTEXT("value " << i)
                {
                    BOOST_TEST(g.get_inrange(fixture.example_coordinates_inside_grid[i]) == fixture.examples_value[i]);
                }
            }
        }
    }

    BOOST_AUTO_TEST_CASE(grid_set_reject_out_of_range_tc) {
        MiscFixture fixture;

        f::Grid g(fixture.dummy_header);

        // Insert some example data
        std::vector<long int> index_buffer(g.num_dims);
        for(int i = 0; i < fixture.example_coordinates_inside_grid.size(); i++) {
            g.set_reject_out_of_range(fixture.example_coordinates_inside_grid[i], fixture.examples_value[i],index_buffer);
        }

        for(int i = 0; i < fixture.example_coordinates_inside_grid.size(); i++) {
            BOOST_TEST_CONTEXT("value " << i)
            {
                BOOST_TEST(g.get_inrange(fixture.example_coordinates_inside_grid[i], index_buffer) == fixture.examples_value[i]);
            }
        }

        // Typically this does nothing
        for(int i = 0; i < fixture.example_coordinates_outside_grid.size(); i++) {
            g.set_reject_out_of_range(fixture.example_coordinates_outside_grid[i], 0.0,index_buffer);
        }

        for(int i = 0; i < fixture.example_coordinates_inside_grid.size(); i++) {
            BOOST_TEST_CONTEXT("value " << i)
            {
                BOOST_TEST(g.get_inrange(fixture.example_coordinates_inside_grid[i], index_buffer) == fixture.examples_value[i]);
            }
        }

    }


    BOOST_AUTO_TEST_CASE(grid_check_inrange_tc) {
        MiscFixture fixture;

        f::Grid g(fixture.dummy_header);

        // Insert some example data
        std::vector<long int> index_buffer(g.num_dims);
        for(int i = 0; i < fixture.example_coordinates_inside_grid.size(); i++) {
            BOOST_TEST_CONTEXT("value " << i)
            {
                BOOST_TEST(g.check_inrange(fixture.example_coordinates_inside_grid[i]));
            }
        }

        // Insert some example data
        for(int i = 0; i < fixture.example_coordinates_outside_grid.size(); i++) {
            BOOST_TEST_CONTEXT("value " << i)
            {
                BOOST_TEST(!g.check_inrange(fixture.example_coordinates_outside_grid[i]));
            }
        }

    }


BOOST_AUTO_TEST_SUITE_END();