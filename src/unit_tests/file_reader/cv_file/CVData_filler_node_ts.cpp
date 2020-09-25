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
#include "../../../file_reader/cv_file/CVData.hpp"
#include "../../../file_reader/cv_file/CVData_filler_node.hpp"


namespace f = fesutils;

BOOST_AUTO_TEST_SUITE(cv_data_filler_node_ts)

    BOOST_AUTO_TEST_CASE(ctor_tc, *boost::unit_test::tolerance(0.001)) {

        std::shared_ptr<f::CVData> im_cvdata =  f::CVData_factory(f::CVData_storage_class::inmemory,
                                                                  2,
                                                                  1,
                                                                  10);
        std::vector<size_t> cv_var_idx = {0, 1};
        size_t metad_bias_var_idx = 2;
        std::vector<size_t> other_vars_idx = {};

        f::CVData_filler_node(im_cvdata,cv_var_idx,metad_bias_var_idx,other_vars_idx);

        BOOST_TEST(true); // Only need to not throw and get here
    }

    BOOST_AUTO_TEST_CASE(insertion_tc, *boost::unit_test::tolerance(0.001)) {

        std::shared_ptr<f::CVData> im_cvdata =  f::CVData_factory(f::CVData_storage_class::inmemory,
                                                                  2,
                                                                  1,
                                                                  10);

        BOOST_REQUIRE(im_cvdata->get_num_record() == 0);


        std::vector<size_t> cv_var_idx = {0, 1};
        size_t metad_bias_var_idx = 2;
        std::vector<size_t> other_vars_idx = {};

        f::CVData_filler_node filler_node(im_cvdata, cv_var_idx, metad_bias_var_idx, other_vars_idx);

        auto data_array = std::make_shared<Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>(2,3);

        (*data_array)(0,0) = 0.1;
        (*data_array)(0,1) = 0.2;
        (*data_array)(0,2) = 0.3;

        (*data_array)(1,0) = 0.1;
        (*data_array)(1,1) = 0.2;
        (*data_array)(1,2) = 0.3;


        filler_node(data_array);

        BOOST_REQUIRE(im_cvdata->get_num_record() == 2);
    }

    BOOST_AUTO_TEST_CASE(insertion_with_supplemental_vars_tc, *boost::unit_test::tolerance(0.001)) {

        std::shared_ptr<f::CVData> im_cvdata =  f::CVData_factory(f::CVData_storage_class::inmemory,
                                                                  2,
                                                                  2,
                                                                  10);

        BOOST_REQUIRE(im_cvdata->get_num_record() == 0);


        std::vector<size_t> cv_var_idx = {0, 1};
        size_t metad_bias_var_idx = 2;
        std::vector<size_t> other_vars_idx = {3};

        f::CVData_filler_node filler_node(im_cvdata, cv_var_idx, metad_bias_var_idx, other_vars_idx);

        auto data_array = std::make_shared<Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>(2,4);

        (*data_array)(0,0) = 0.1;
        (*data_array)(0,1) = 0.2;
        (*data_array)(0,2) = 0.3;
        (*data_array)(0,3) = 0.4;

        (*data_array)(1,0) = 0.1;
        (*data_array)(1,1) = 0.2;
        (*data_array)(1,2) = 0.3;
        (*data_array)(1,3) = 0.4;

        filler_node(data_array);

        BOOST_REQUIRE(im_cvdata->get_num_record() == 2);
    }


BOOST_AUTO_TEST_SUITE_END();