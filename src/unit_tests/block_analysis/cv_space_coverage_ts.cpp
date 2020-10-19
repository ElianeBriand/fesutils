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

#include "../../GeneralOptions.h"
#include "../../file_reader/cv_file/CVData.hpp"
#include "../../file_reader/grid_file/GridData.hpp"
#include "../../block_analysis/cv_space_coverage.hpp"

namespace f = fesutils;

BOOST_AUTO_TEST_SUITE(cv_space_coverage_ts)

    BOOST_AUTO_TEST_CASE(coverage_basic_tc, *boost::unit_test::tolerance(0.001)) {

        // PRELUDE
        const unsigned int num_axis = 2;

        std::shared_ptr<f::CVData> im_cvdata =  CVData_factory(f::CVData_storage_class::inmemory,
                                                               num_axis,
                                                               1,
                                                               10);

        BOOST_REQUIRE(im_cvdata->get_num_record() == 0);

        std::vector<double> datapoint_vector = {0.1, 2.0, 0.3,
                                                0.1, 2.5, 0.8,
                                                0.2, 4.0, 420.69};
        im_cvdata->insertDatapointsVector(datapoint_vector);
        BOOST_REQUIRE(im_cvdata->get_num_record() == 3);
        
        std::vector<unsigned int> bins_per_axis = {5+1, 2+1};
        std::vector<double> min_bin_value_per_axis = {0.0, 2.0};
        std::vector<double> max_bin_value_per_axis = {0.5, 4.0};
        bool track_access_number = false;
        std::shared_ptr<f::GridData> gd = f::GridData_factory(f::GridData_storage_class::inmemory,
                                                              num_axis,
                                                              bins_per_axis,
                                                              min_bin_value_per_axis,
                                                              max_bin_value_per_axis,
                                                              track_access_number);



        // TEST

        std::shared_ptr<f::GridAccessTracker> gat = f::count_sampled_voxel_in_CVData(*im_cvdata, *gd);

        bool all_voxel_accessed_1_or_less = gat->all_voxel_accessed_n_times_or_less(1);

        BOOST_TEST(all_voxel_accessed_1_or_less);





    }


BOOST_AUTO_TEST_SUITE_END();