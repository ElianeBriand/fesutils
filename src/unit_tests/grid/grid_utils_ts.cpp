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

namespace f = fesutils;
namespace tt = boost::test_tools;

//, *boost::unit_test::tolerance(0.001)

BOOST_AUTO_TEST_SUITE(grid_utils_ts)

    BOOST_AUTO_TEST_CASE(find_fields_index_that_have_required_attribute_for_grid_tc) {
        MiscFixture fixture;

        std::vector<size_t> indexes = f::find_fields_index_that_have_required_attribute_for_grid(fixture.dummy_header);

        BOOST_TEST(indexes == fixture.expected_grid_field_indexes, tt::per_element());

    }

    BOOST_AUTO_TEST_CASE(grid_dims_from_header_tc) {
        MiscFixture fixture;

        std::vector<size_t> which_fields = f::find_fields_index_that_have_required_attribute_for_grid(fixture.dummy_header);

        std::vector<long int> dims = f::grid_dims_from_header(fixture.dummy_header, which_fields);

        BOOST_TEST(dims == fixture.expected_grid_dims, tt::per_element());

    }

    BOOST_AUTO_TEST_CASE(grid_dims_from_header_incorrect_nbins_tc) {
        MiscFixture fixture;
        fixture.dummy_header.fields.front().attributes["nbins"] = 0;

        std::vector<size_t> which_fields = f::find_fields_index_that_have_required_attribute_for_grid(fixture.dummy_header);

        BOOST_CHECK_THROW(f::grid_dims_from_header(fixture.dummy_header, which_fields), std::runtime_error );
    }


    BOOST_AUTO_TEST_CASE(grid_bin_sizes_from_header_tc) {
        MiscFixture fixture;

        std::vector<size_t> which_fields = f::find_fields_index_that_have_required_attribute_for_grid(fixture.dummy_header);

        std::vector<double> bin_sizes = f::grid_bin_sizes_from_header(fixture.dummy_header, which_fields);

        BOOST_TEST(bin_sizes == fixture.expected_grid_bin_sizes, tt::per_element());

    }

    BOOST_AUTO_TEST_CASE(grid_bin_sizes_from_header_incorrect_nbins_tc) {
        MiscFixture fixture;
        fixture.dummy_header.fields.front().attributes["nbins"] = 0;

        std::vector<size_t> which_fields = f::find_fields_index_that_have_required_attribute_for_grid(fixture.dummy_header);

        BOOST_CHECK_THROW(f::grid_bin_sizes_from_header(fixture.dummy_header, which_fields), std::runtime_error );
    }

    BOOST_AUTO_TEST_CASE(extract_grid_min_val_tc) {
        MiscFixture fixture;

        std::vector<size_t> which_fields = f::find_fields_index_that_have_required_attribute_for_grid(fixture.dummy_header);

        std::vector<double> min_vals = f::extract_grid_min_val(fixture.dummy_header, which_fields);

        BOOST_TEST(min_vals == fixture.expected_grid_min_vals, tt::per_element());

    }

    BOOST_AUTO_TEST_CASE(extract_grid_max_val_tc) {
        MiscFixture fixture;

        std::vector<size_t> which_fields = f::find_fields_index_that_have_required_attribute_for_grid(fixture.dummy_header);

        std::vector<double> max_vals = f::extract_grid_max_val(fixture.dummy_header, which_fields);

        BOOST_TEST(max_vals == fixture.expected_grid_max_vals, tt::per_element());

    }

    BOOST_AUTO_TEST_CASE(compute_grid_bin_edges_tc) {
        MiscFixture fixture;

        std::vector<size_t> which_fields = f::find_fields_index_that_have_required_attribute_for_grid(fixture.dummy_header);
        std::vector<double> min_vals = f::extract_grid_min_val(fixture.dummy_header, which_fields);
        std::vector<double> max_vals = f::extract_grid_max_val(fixture.dummy_header, which_fields);
        std::vector<double> bin_sizes = f::grid_bin_sizes_from_header(fixture.dummy_header, which_fields);
        std::vector<long int> dims = f::grid_dims_from_header(fixture.dummy_header, which_fields);

        std::vector<std::vector<double>> bin_edges = f::compute_grid_bin_edges(min_vals,max_vals, bin_sizes, dims);

        if(bin_edges.size() == fixture.expected_grid_bin_edges.size())
        {
            for(int i=0; i < bin_edges.size(); i++)
            {
                BOOST_TEST_CONTEXT("dim " << i)
                {
                    BOOST_TEST(bin_edges[i] == fixture.expected_grid_bin_edges[i], tt::per_element());
                }
            }
        }
        else
        {
            BOOST_TEST(bin_edges.size() == fixture.expected_grid_bin_edges.size());
        }

    }

    BOOST_AUTO_TEST_CASE(compute_grid_bin_center_tc) {
        MiscFixture fixture;

        std::vector<size_t> which_fields = f::find_fields_index_that_have_required_attribute_for_grid(fixture.dummy_header);
        std::vector<double> min_vals = f::extract_grid_min_val(fixture.dummy_header, which_fields);
        std::vector<double> max_vals = f::extract_grid_max_val(fixture.dummy_header, which_fields);
        std::vector<double> bin_sizes = f::grid_bin_sizes_from_header(fixture.dummy_header, which_fields);
        std::vector<long int> dims = f::grid_dims_from_header(fixture.dummy_header, which_fields);
        std::vector<std::vector<double>> bin_edges = f::compute_grid_bin_edges(min_vals,max_vals, bin_sizes, dims);

        std::vector<std::vector<double>> bin_centers = f::compute_grid_bin_center(bin_edges);

        if(bin_centers.size() == fixture.expected_grid_bin_centers.size())
        {
            for(int i=0; i < bin_centers.size(); i++)
            {
                BOOST_TEST_CONTEXT("dim " << i)
                {
                    BOOST_TEST(bin_centers[i] == fixture.expected_grid_bin_centers[i], tt::per_element());
                }
            }
        }
        else
        {
            BOOST_TEST(bin_centers.size() == fixture.expected_grid_bin_centers.size());
        }

    }

BOOST_AUTO_TEST_SUITE_END();