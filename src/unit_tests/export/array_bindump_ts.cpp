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
#include <iomanip>
#include "../TempDirectory.hpp"
#include "../../export/array_bindump.hpp"


namespace f = fesutils;

std::string ToHex(const std::string& s, bool upper_case)
{
    std::ostringstream ret;

    for (std::string::size_type i = 0; i < s.length(); ++i)
    {
        int z = s[i]&0xff;
        ret << std::hex << std::setfill('0') << std::setw(2) << (upper_case ? std::uppercase : std::nouppercase) << z;
    }

    return ret.str();
}


BOOST_AUTO_TEST_SUITE(array_bindump_ts)

    BOOST_AUTO_TEST_CASE(dummy_test_case_working_tc, *boost::unit_test::tolerance(0.001)) {
        TempDirectory td;
        bf::path td_path = td.getDirPath();

        // Endiannes require reversing 32 blocks (FIXME ?)
        std::vector<uint8_t> raw_valid_bindump = {0x01, 0x01, 0x5D, 0xFE,  // Magic number
                                                  16, 0x00, 0x00, 0x00, // start-of-data offset
                                                  0x01, 0x00, 0x00, 0x00, // Format version: 1
                                                  0x05, 0x00, 0x00, 0x00, // Size of original file in byte
                                                  0xDE, 0xAD, 0xBE, 0xEF // Dummy data
                                                  };

        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("dummy", "AAAAA");
        bf::path temp_file_path = temp_file->getFilePath();
        std::shared_ptr<TempTextFile>  temp_file_bd = td.createTemporaryFileInDir("dummy.bindump", raw_valid_bindump);
        bf::path temp_dbfile_path = temp_file_bd->getFilePath();

        std::string tohexed = ToHex(std::string((char*) raw_valid_bindump.data(), raw_valid_bindump.size()), true);
        std::cout << tohexed << std::endl;

        std::optional<bf::path> maybe_bindump_path = f::corresponding_bindump_exists(temp_file_path);

        BOOST_TEST(maybe_bindump_path.has_value());

    }

    BOOST_AUTO_TEST_CASE(dummy_test_case_nonmagicvalue_tc, *boost::unit_test::tolerance(0.001)) {
        TempDirectory td;
        bf::path td_path = td.getDirPath();

        std::vector<uint8_t> raw_valid_bindump = {0xFF, 0xFF, 0x5D, 0xFE,  // Magic number INCORRECT
                                                  16, 0x00, 0x00, 0x00, // start-of-data offset
                                                  0x01, 0x00, 0x00, 0x00, // Format version: 1
                                                  0x05, 0x00, 0x00, 0x00, // Size of original file in byte
                                                  0xDE, 0xAD, 0xBE, 0xEF // Dummy data
        };

        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("dummy", "AAAAA");
        bf::path temp_file_path = temp_file->getFilePath();
        std::shared_ptr<TempTextFile>  temp_file_bd = td.createTemporaryFileInDir("dummy.bindump", raw_valid_bindump);
        bf::path temp_dbfile_path = temp_file_bd->getFilePath();

        std::string tohexed = ToHex(std::string((char*) raw_valid_bindump.data(), raw_valid_bindump.size()), true);
        std::cout << tohexed << std::endl;

        std::optional<bf::path> maybe_bindump_path = f::corresponding_bindump_exists(temp_file_path);

        BOOST_TEST(!maybe_bindump_path.has_value());

    }

    BOOST_AUTO_TEST_CASE(dummy_test_case_badversion_tc, *boost::unit_test::tolerance(0.001)) {
        TempDirectory td;
        bf::path td_path = td.getDirPath();

        std::vector<uint8_t> raw_valid_bindump = {0x01, 0x01, 0x5D, 0xFE,  // Magic number
                                                  16, 0x00, 0x00, 0x00, // start-of-data offset
                                                  51, 0x00, 0x00, 0x00, // Format version: 51, will not parse
                                                  0x05, 0x00, 0x00, 0x00, // Size of original file in byte
                                                  0xDE, 0xAD, 0xBE, 0xEF // Dummy data
        };

        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("dummy", "AAAAA");
        bf::path temp_file_path = temp_file->getFilePath();
        std::shared_ptr<TempTextFile>  temp_file_bd = td.createTemporaryFileInDir("dummy.bindump", raw_valid_bindump);
        bf::path temp_dbfile_path = temp_file_bd->getFilePath();

        std::string tohexed = ToHex(std::string((char*) raw_valid_bindump.data(), raw_valid_bindump.size()), true);
        std::cout << tohexed << std::endl;

        std::optional<bf::path> maybe_bindump_path = f::corresponding_bindump_exists(temp_file_path);

        BOOST_TEST(!maybe_bindump_path.has_value());

    }


    BOOST_AUTO_TEST_CASE(dummy_test_case_badsize_tc, *boost::unit_test::tolerance(0.001)) {
        TempDirectory td;
        bf::path td_path = td.getDirPath();

        std::vector<uint8_t> raw_valid_bindump = {0x01, 0x01, 0x5D, 0xFE,  // Magic number
                                                  16, 0x00, 0x00, 0x00, // start-of-data offset
                                                  1, 0x00, 0x00, 0x00, // Format version: 1
                                                  0x06, 0x00, 0x00, 0x00, // Size of original file in byte (INCORRECT)
                                                  0xDE, 0xAD, 0xBE, 0xEF // Dummy data
        };

        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("dummy", "AAAAA");
        bf::path temp_file_path = temp_file->getFilePath();
        std::shared_ptr<TempTextFile>  temp_file_bd = td.createTemporaryFileInDir("dummy.bindump", raw_valid_bindump);
        bf::path temp_dbfile_path = temp_file_bd->getFilePath();

        std::string tohexed = ToHex(std::string((char*) raw_valid_bindump.data(), raw_valid_bindump.size()), true);
        std::cout << tohexed << std::endl;

        std::optional<bf::path> maybe_bindump_path = f::corresponding_bindump_exists(temp_file_path);

        BOOST_TEST(!maybe_bindump_path.has_value());

    }

    BOOST_AUTO_TEST_CASE(dummy_test_case_nobindumpfile_tc, *boost::unit_test::tolerance(0.001)) {
        TempDirectory td;
        bf::path td_path = td.getDirPath();

        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("dummy", "AAAAA");
        bf::path temp_file_path = temp_file->getFilePath();

        std::optional<bf::path> maybe_bindump_path = f::corresponding_bindump_exists(temp_file_path);

        BOOST_TEST(!maybe_bindump_path.has_value());
    }


BOOST_AUTO_TEST_SUITE_END();