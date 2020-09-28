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
#include "../../TempDirectory.hpp"
#include "../../../GeneralOptions.h"
#include "../../../file_reader/read_file_header.hpp"
#include "../../../file_reader/grid_file/GridData.hpp"
#include "../../../file_reader/grid_file/ingest_grid_data.hpp"
#include "../../../common/NotImplementedError.hpp"

namespace {
    std::string example_grid_file = "#! FIELDS fragment_position.x fragment_position.y fragment_position.z metad_position.bias der_fragment_position.x der_fragment_position.y der_fragment_position.z\n"
                                    "#! SET min_fragment_position.x 0.88\n"
                                    "#! SET max_fragment_position.x 10.06\n"
                                    "#! SET nbins_fragment_position.x  185\n"
                                    "#! SET periodic_fragment_position.x false\n"
                                    "#! SET min_fragment_position.y 3.09\n"
                                    "#! SET max_fragment_position.y 14.22\n"
                                    "#! SET nbins_fragment_position.y  224\n"
                                    "#! SET periodic_fragment_position.y false\n"
                                    "#! SET min_fragment_position.z -1.86\n"
                                    "#! SET max_fragment_position.z 9.70\n"
                                    "#! SET nbins_fragment_position.z  233\n"
                                    "#! SET periodic_fragment_position.z false\n"
                                    "    0.880000000    3.090000000   -1.860000000   11.000000000    0.000000000    0.000000000    0.000000000\n"
                                    "    0.929891304    3.090000000   -1.860000000    2.000000000    0.000000000    0.000000000    0.000000000\n"
                                    "    0.979782609    3.090000000   -1.860000000    1.200000000    0.000000000    0.000000000    0.000000000\n"
                                    "    1.029673913    3.090000000   -1.860000000  253.000000000    0.000000000    0.000000000    0.000000000\n"
                                    "    1.079565217    3.090000000   -1.860000000    1.100000000    0.000000000    0.000000000    0.000000000\n"
                                    "    1.129456522    3.090000000   -1.860000000    8.800000000    0.000000000    0.000000000    0.000000000\n"
                                    "    1.179347826    3.090000000   -1.860000000    1.400000000    0.000000000    0.000000000    0.000000000\n"
                                    "    1.229239130    3.090000000   -1.860000000    0.000000000    0.000000000    0.000000000    0.000000000\n"
                                    "    1.279130435    3.090000000   -1.860000000    0.000000000    0.000000000    0.000000000    0.000000000\n"
                                    "    9.511195652   14.220000000    9.700000000    0.000000000    0.000000000    0.000000000    0.000000000\n"
                                    "    9.561086957   14.220000000    9.700000000    0.000000000    0.000000000    0.000000000    0.000000000\n"
                                    "    9.610978261   14.220000000    9.700000000    48.90000000    0.000000000    0.000000000    0.000000000\n"
                                    "    9.660869565   14.220000000    9.700000000    0.000000000    0.000000000    0.000000000    0.000000000\n"
                                    "    9.710760870   14.220000000    9.700000000    0.000000000    0.000000000    0.000000000    0.000000000\n"
                                    "    9.760652174   14.220000000    9.700000000    0.000000000    0.000000000    0.000000000    0.000000000\n"
                                    "    9.810543478   14.220000000    9.700000000    0.000000000    0.000000000    0.000000000    0.000000000\n"
                                    "    9.860434783   14.220000000    9.700000000    0.000000000    0.000000000    0.000000000    0.000000000\n"
                                    "    9.910326087   14.220000000    9.700000000    0.000000000    0.000000000    0.000000000    0.000000000\n"
                                    "    9.960217391   14.220000000    9.700000000    0.000000000    0.000000000    0.000000000    0.000000000\n"
                                    "   10.010108696   14.220000000    9.700000000    0.000000000    0.000000000    0.000000000    0.000000000\n"
                                    "   10.060000000   14.220000000    9.700000000    0.000000000    0.000000000    0.000000000    0.000000000\n";

    std::vector<uint8_t> raw_valid_grid_bindump = {0x01, 0x01, 0x5D, 0xFE,  // Magic number
                                              16, 0x00, 0x00, 0x00, // start-of-data offset
                                              1, 0x00, 0x00, 0x00, // Format version: 1
                                              0x1F, 0x0B, 0x00, 0x00, // Size of original file in byte
                                              0xDE, 0xAD, 0xBE, 0xEF // Dummy data
    };
}

namespace f = fesutils;

BOOST_AUTO_TEST_SUITE(ingest_grid_data_ts)

    BOOST_AUTO_TEST_CASE(ingest_from_bindump_tc, *boost::unit_test::tolerance(0.001)) {
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("GRID_fragment_test",
                                                                               example_grid_file);
        bf::path grid_file_path = temp_file->getFilePath();

        std::shared_ptr<TempTextFile>  temp_bindump = td.createTemporaryFileInDir("GRID_fragment_test.bindump",
                                                                                  raw_valid_grid_bindump);
        bf::path bindump_path = temp_bindump->getFilePath();



        f::GeneralOptions options;

        f::PlumedDatHeader header = f::read_plumed_file_header(temp_file->getName());

        // Ensure that the fake bindump has the correct size in the 'original text file size' field
        BOOST_TEST(example_grid_file.size() == 2847);
        BOOST_REQUIRE(example_grid_file.size() == 2847);

        BOOST_CHECK_THROW(
        f::ingest_grid_data(options,grid_file_path,header,3,{0,1,2}), not_implemented_error);


    }

    BOOST_AUTO_TEST_CASE(ingest_from_textfile_tc, *boost::unit_test::tolerance(0.001)) {
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("GRID_fragment_test",
                                                                               example_grid_file);
        bf::path grid_file_path = temp_file->getFilePath();



        f::GeneralOptions options;

        f::PlumedDatHeader header = f::read_plumed_file_header(temp_file->getName());

        std::shared_ptr<f::GridData> grid =
                f::ingest_grid_data(options,grid_file_path,header,3,{0,1,2});

        BOOST_REQUIRE(grid != nullptr);

        std::vector<double> coord = {9.611, 14.22, 9.7};
        double value = 0.0;
        std::vector<long> idx_buffer(3,0);
        grid->get_value_at_coord_rangechecked(coord, value, idx_buffer);

        BOOST_TEST(value == 48.9);

        coord = {0.98, 3.09, -1.86};
        value = 152.2;
        idx_buffer = {0, 0, 0};
        grid->get_value_at_coord_rangechecked(coord, value, idx_buffer);

        BOOST_TEST(value == 1.2);

    }


BOOST_AUTO_TEST_SUITE_END();