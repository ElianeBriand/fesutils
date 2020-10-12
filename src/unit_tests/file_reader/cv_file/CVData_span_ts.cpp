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

namespace f = fesutils;

BOOST_AUTO_TEST_SUITE(CVData_span_ts)

    BOOST_AUTO_TEST_CASE(constructor_and_accessor_tc, *boost::unit_test::tolerance(0.001)) {
        std::shared_ptr<f::CVData> im_cvdata =  CVData_factory(f::CVData_storage_class::inmemory,
                                                               2,
                                                               1,
                                                               10);

        BOOST_REQUIRE(im_cvdata->get_num_record() == 0);

        std::vector<double> datapoint = {0.1, 0.2, 0.3};

        im_cvdata->insertDatapoint(datapoint);

        datapoint = {0.4, 0.5, 0.6};
        im_cvdata->insertDatapoint(datapoint);
        datapoint = {0.7, 0.8, 0.9};
        im_cvdata->insertDatapoint(datapoint);
        datapoint = {1.0, 1.1, 1.2};
        im_cvdata->insertDatapoint(datapoint);
        datapoint = {1.3, 1.4, 1.5};
        im_cvdata->insertDatapoint(datapoint);

        BOOST_REQUIRE(im_cvdata->get_num_record() == 5);

        f::CVData_span span_1(im_cvdata.get(),0, 5);

        BOOST_TEST(span_1.get_begin_offset() == 0);
        BOOST_TEST(span_1.get_end_offset() == 5);
        BOOST_TEST(span_1.get_actual_size() == 5);
        BOOST_TEST(span_1.is_requested_span_size());

        f::CVData_span span_2(im_cvdata.get(), 3, 5);

        BOOST_TEST(span_2.get_begin_offset() == 3);
        BOOST_TEST(span_2.get_end_offset() == 5);
        BOOST_TEST(span_2.get_actual_size() == 2);
        BOOST_TEST(!span_2.is_requested_span_size());

        BOOST_CHECK_THROW(f::CVData_span(nullptr,0, 5), std::runtime_error);

        BOOST_CHECK_THROW(f::CVData_span(im_cvdata.get(),8, 5), std::runtime_error);

        f::CVData_span span_3(im_cvdata.get(), 0, -1);
        BOOST_TEST(span_3.get_begin_offset() == 0);
        BOOST_TEST(span_3.get_end_offset() == 5);
        BOOST_TEST(span_3.get_actual_size() == 5);
        BOOST_TEST(span_3.is_requested_span_size());

        BOOST_CHECK_THROW(f::CVData_span(im_cvdata.get(),0, -10), std::runtime_error);



    }



BOOST_AUTO_TEST_SUITE_END();