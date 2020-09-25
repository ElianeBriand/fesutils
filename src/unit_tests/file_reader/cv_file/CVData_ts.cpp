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
#include "../../../common/NotImplementedError.hpp"


namespace f = fesutils;

BOOST_AUTO_TEST_SUITE(cv_data_ts)

    BOOST_AUTO_TEST_CASE(cv_data_ctor_tc, *boost::unit_test::tolerance(0.001)) {


        std::shared_ptr<f::CVData> im_cvdata =  f::CVData_factory(f::CVData_storage_class::inmemory,
                                               2,
                                               1,
                                               10);

        BOOST_TEST(im_cvdata->get_num_cv_dimensions() == 2);
        BOOST_TEST(im_cvdata->get_num_value_dimensions() == 1);
        BOOST_TEST(im_cvdata->get_total_dim_in_one_record() == 2+1);
        BOOST_TEST(im_cvdata->get_num_record() == 0);

        // Check not implemented error
        BOOST_CHECK_THROW(f::CVData_factory(f::CVData_storage_class::disk,
                                            2,
                                            1,
                                            10), not_implemented_error);

        BOOST_CHECK_THROW(f::CVData_factory(f::CVData_storage_class::diskandcache,
                                            2,
                                            1,
                                            10), not_implemented_error);


    }

    BOOST_AUTO_TEST_CASE(cv_data_insert_datapoint_tc, *boost::unit_test::tolerance(0.001)) {


        std::shared_ptr<f::CVData> im_cvdata =  CVData_factory(f::CVData_storage_class::inmemory,
                                                               2,
                                                               1,
                                                               10);

        BOOST_TEST(im_cvdata->get_num_record() == 0);

        std::vector<double> datapoint = {0.1, 0.2, 0.3};

        im_cvdata->insertDatapoint(datapoint);

        BOOST_TEST(im_cvdata->get_num_record() == 1);

        std::vector<double> bad_datapoint = {0.1, 0.2, 0.3, 0.4};

        BOOST_CHECK_THROW(im_cvdata->insertDatapoint(bad_datapoint), std::runtime_error);

    }

    BOOST_AUTO_TEST_CASE(cv_data_insert_datapoint_vector_tc, *boost::unit_test::tolerance(0.001)) {


        std::shared_ptr<f::CVData> im_cvdata =  CVData_factory(f::CVData_storage_class::inmemory,
                                                               2,
                                                               1,
                                                               10);

        BOOST_TEST(im_cvdata->get_num_record() == 0);

        std::vector<double> datapoint_vector = {0.1, 0.2, 0.3, 0.1, 0.2, 0.3};

        im_cvdata->insertDatapointsVector(datapoint_vector);

        BOOST_TEST(im_cvdata->get_num_record() == 2);

        std::vector<double> bad_datapoint = {0.1, 0.2, 0.3, 0.1, 0.2, 0.3, 0.4};

        BOOST_CHECK_THROW(im_cvdata->insertDatapointsVector(bad_datapoint), std::runtime_error);

    }


BOOST_AUTO_TEST_SUITE_END();