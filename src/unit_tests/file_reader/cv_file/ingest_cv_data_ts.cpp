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
#include "../../../file_reader/cv_file/ingest_cv_data.hpp"
#include "../../TempDirectory.hpp"
#include "../../../file_reader/read_file_header.hpp"
#include "../../../common/NotImplementedError.hpp"


namespace f = fesutils;

namespace {
    std::string example_cv_file = "#! FIELDS time fragment_position.x fragment_position.y fragment_position.z metad_position.bias wall_lower.bias wall_upper.bias\n"
                                  " 0.000000 5.077540 7.494030 3.637163 0.000000 0.000000 0.000000\n"
                                  " 0.020000 5.080074 7.491562 3.632814 0.000000 0.000000 0.000000\n";

    std::vector<uint8_t> raw_valid_bindump = {0x01, 0x01, 0x5D, 0xFE,  // Magic number
                                              16, 0x00, 0x00, 0x00, // start-of-data offset
                                              1, 0x00, 0x00, 0x00, // Format version: 1
                                              255, 0x00, 0x00, 0x00, // Size of original file in byte
                                              0xDE, 0xAD, 0xBE, 0xEF // Dummy data
    };
}

BOOST_AUTO_TEST_SUITE(ingest_cv_data_ts)

    BOOST_AUTO_TEST_CASE(ingest_bindump_tc, *boost::unit_test::tolerance(0.001)) {
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("CV_fragment_test",
                                                                               example_cv_file);
        bf::path cv_file_path = temp_file->getFilePath();

        std::shared_ptr<TempTextFile>  temp_bindump = td.createTemporaryFileInDir("CV_fragment_test.bindump",
                                                                                  raw_valid_bindump);
        bf::path bindump_path = temp_bindump->getFilePath();



        f::GeneralOptions options;

        std::shared_ptr<f::CVData> im_cvdata =  f::CVData_factory(f::CVData_storage_class::inmemory,
                                                                  3,
                                                                  1,
                                                                  10);

        f::PlumedDatHeader header = f::read_plumed_file_header(temp_file->getName());
        std::vector<std::string> cv_varnames = {"fragment_position.x", "fragment_position.y", "fragment_position.z"};
        std::vector<size_t> cv_vars_idx = {1,2,3};
        std::string metad_bias_varname = "metad_position.bias";
        size_t metad_bias_var_idx = 4;
        std::vector<std::string> other_varnames = {"wall_lower.bias", "wall_upper.bias"};
        std::vector<size_t> other_var_idx = {5, 6};

        BOOST_CHECK_THROW(ingest_cv_data_from_bindump(options,
                                               cv_file_path,
                                               im_cvdata,
                                                header,
                                                cv_varnames,
                                               metad_bias_varname,
                                               other_varnames,
                                               cv_vars_idx,
                                               metad_bias_var_idx,
                                               other_var_idx,
                                               bindump_path),
                          not_implemented_error);

        BOOST_CHECK_THROW(ingest_cv_data(options,
                                                      cv_file_path,
                                                      im_cvdata,
                                                      header,
                                                      cv_varnames,
                                                      metad_bias_varname,
                                                      other_varnames),
                          not_implemented_error);

    }

    BOOST_AUTO_TEST_CASE(ingest_textfile_tc, *boost::unit_test::tolerance(0.001)) {
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("CV_fragment_test",
                                                                               example_cv_file);
        bf::path cv_file_path = temp_file->getFilePath();




        f::GeneralOptions options;

        std::shared_ptr<f::CVData> im_cvdata =  f::CVData_factory(f::CVData_storage_class::inmemory,
                                                                  3,
                                                                  3,
                                                                  10);

        f::PlumedDatHeader header = f::read_plumed_file_header(temp_file->getName());
        std::vector<std::string> cv_varnames = {"fragment_position.x", "fragment_position.y", "fragment_position.z"};
        std::vector<size_t> cv_vars_idx = {1,2,3};
        std::string metad_bias_varname = "metad_position.bias";
        size_t metad_bias_var_idx = 4;
        std::vector<std::string> other_varnames = {"wall_lower.bias", "wall_upper.bias"};
        std::vector<size_t> other_var_idx = {5, 6};


        bool success_ingesting = ingest_cv_data(options,
                                         cv_file_path,
                                         im_cvdata,
                                         header,
                                         cv_varnames,
                                         metad_bias_varname,
                                         other_varnames);

        BOOST_TEST(success_ingesting == true);

        BOOST_TEST(im_cvdata->get_num_record() == 2);



    }


BOOST_AUTO_TEST_SUITE_END();