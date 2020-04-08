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
#include <sstream>

#include <fmt/printf.h>

#include <boost/test/unit_test.hpp>

#include "../../file_reader/read_file.hpp"
#include "../../common/conversions.hpp"

#include "../TempTextFile.hpp"

namespace f = fesutils;
namespace tt = boost::test_tools;

BOOST_AUTO_TEST_SUITE(read_file_ts)

    BOOST_AUTO_TEST_CASE(find_eol_indexes_tc) {
        {
            std::vector<uint8_t> buffer = {'A', 'B', '\n', 'C', 'D', 'E', '\n', 'F'};
            std::vector<size_t> eol_indx;

            f::find_eol_indexes(eol_indx, buffer);

            BOOST_TEST(eol_indx.size() == 2);
            BOOST_REQUIRE(eol_indx.size() == 2);
            BOOST_TEST(eol_indx[0] == 2);
            BOOST_TEST(eol_indx[1] == 6);
        }

        {
            std::vector<uint8_t> buffer = {'\n', 'C', 'D', 'E', '\n'};
            std::vector<size_t> eol_indx;

            f::find_eol_indexes(eol_indx, buffer);

            BOOST_TEST(eol_indx.size() == 2);
            BOOST_REQUIRE(eol_indx.size() == 2);
            BOOST_TEST(eol_indx[0] == 0);
            BOOST_TEST(eol_indx[1] == 4);
        }

    }

    BOOST_AUTO_TEST_CASE(line_from_eol_indexes) {

        {
            std::vector<uint8_t> buffer = {'A', 'B', '\n', 'C', 'D', 'E', '\n', 'F'};
            std::vector<size_t> eol_indx;

            f::find_eol_indexes(eol_indx, buffer);

            BOOST_REQUIRE(eol_indx.size() == 2);
            BOOST_REQUIRE(eol_indx[0] == 2);
            BOOST_REQUIRE(eol_indx[1] == 6);

            auto line_sptr = std::make_shared< std::vector<std::string> >();
            std::string remainder = "ZZ";
            size_t processed = 0; size_t error = 0;
            f::line_from_eol_indexes(line_sptr, remainder, buffer, eol_indx, processed, error);

            BOOST_TEST(line_sptr->size() == 2);
            BOOST_REQUIRE(line_sptr->size() == 2);

            BOOST_TEST(processed == 2);
            BOOST_TEST(error == 0);
            BOOST_TEST(line_sptr->at(0) == "ZZAB");
            BOOST_TEST(line_sptr->at(1) == "CDE");
            BOOST_TEST(remainder == "F");
        }

        {
            std::vector<uint8_t> buffer = {'A', 'B', '\n', 'C', 'D', 'E', '\n'};
            std::vector<size_t> eol_indx;

            f::find_eol_indexes(eol_indx, buffer);

            BOOST_REQUIRE(eol_indx.size() == 2);
            BOOST_REQUIRE(eol_indx[0] == 2);
            BOOST_REQUIRE(eol_indx[1] == 6);

            auto line_sptr = std::make_shared< std::vector<std::string> >();
            std::string remainder = "ZZ";
            size_t processed = 0; size_t error = 0;
            f::line_from_eol_indexes(line_sptr, remainder, buffer, eol_indx, processed, error);

            BOOST_TEST(line_sptr->size() == 2);
            BOOST_REQUIRE(line_sptr->size() == 2);
            BOOST_TEST(processed == 2);
            BOOST_TEST(error == 0);
            BOOST_TEST(line_sptr->at(0) == "ZZAB");
            BOOST_TEST(line_sptr->at(1) == "CDE");
            BOOST_TEST(remainder == "");
        }

        {
            std::vector<uint8_t> buffer = {'A', 'B', '\n', 'C', 'D', 'E', '\n', 'F'};
            std::vector<size_t> eol_indx;

            f::find_eol_indexes(eol_indx, buffer);

            BOOST_REQUIRE(eol_indx.size() == 2);
            BOOST_REQUIRE(eol_indx[0] == 2);
            BOOST_REQUIRE(eol_indx[1] == 6);

            auto line_sptr = std::make_shared< std::vector<std::string> >();
            std::string remainder = "";
            size_t processed = 0; size_t error = 0;
            f::line_from_eol_indexes(line_sptr, remainder, buffer, eol_indx, processed, error);

            BOOST_TEST(line_sptr->size() == 2);
            BOOST_REQUIRE(line_sptr->size() == 2);
            BOOST_TEST(processed == 2);
            BOOST_TEST(error == 0);
            BOOST_TEST(line_sptr->at(0) == "AB");
            BOOST_TEST(line_sptr->at(1) == "CDE");
            BOOST_TEST(remainder == "F");
        }

        {
            std::vector<uint8_t> buffer = {'A', 'B', '\n', 'C', 'D', 'E', '\n'};
            std::vector<size_t> eol_indx;

            f::find_eol_indexes(eol_indx, buffer);

            BOOST_REQUIRE(eol_indx.size() == 2);
            BOOST_REQUIRE(eol_indx[0] == 2);
            BOOST_REQUIRE(eol_indx[1] == 6);

            auto line_sptr = std::make_shared< std::vector<std::string> >();
            std::string remainder = "";
            size_t processed = 0; size_t error = 0;
            f::line_from_eol_indexes(line_sptr, remainder, buffer, eol_indx, processed, error);

            BOOST_TEST(line_sptr->size() == 2);
            BOOST_REQUIRE(line_sptr->size() == 2);
            BOOST_TEST(processed == 2);
            BOOST_TEST(error == 0);
            BOOST_TEST(line_sptr->at(0) == "AB");
            BOOST_TEST(line_sptr->at(1) == "CDE");
            BOOST_TEST(remainder == "");
        }

        {
            std::vector<uint8_t> buffer = {'A', 'B', '\n', '#', 'D', 'E', '\n'};
            std::vector<size_t> eol_indx;

            f::find_eol_indexes(eol_indx, buffer);

            BOOST_REQUIRE(eol_indx.size() == 2);
            BOOST_REQUIRE(eol_indx[0] == 2);
            BOOST_REQUIRE(eol_indx[1] == 6);

            auto line_sptr = std::make_shared< std::vector<std::string> >();
            std::string remainder = "";
            size_t processed = 0; size_t error = 0;
            f::line_from_eol_indexes(line_sptr, remainder, buffer, eol_indx, processed, error);

            BOOST_TEST(line_sptr->size() == 1);
            BOOST_REQUIRE(line_sptr->size() == 1);
            BOOST_TEST(processed == 2);
            BOOST_TEST(error == 1);
            BOOST_TEST(line_sptr->at(0) == "AB");
            BOOST_TEST(remainder == "");
        }

        {
            std::vector<uint8_t> buffer = {'A', 'B', '\n', 'C', 'D', 'E'};
            std::vector<size_t> eol_indx;

            f::find_eol_indexes(eol_indx, buffer);

            BOOST_REQUIRE(eol_indx.size() == 1);
            BOOST_REQUIRE(eol_indx[0] == 2);

            auto line_sptr = std::make_shared< std::vector<std::string> >();
            std::string remainder = "";
            size_t processed = 0; size_t error = 0;
            f::line_from_eol_indexes(line_sptr, remainder, buffer, eol_indx, processed, error);

            BOOST_TEST(line_sptr->size() == 1);
            BOOST_REQUIRE(line_sptr->size() == 1);
            BOOST_TEST(processed == 1);
            BOOST_TEST(error == 0);
            BOOST_TEST(line_sptr->at(0) == "AB");
            BOOST_TEST(remainder == "CDE");
        }

        {
            std::vector<uint8_t> buffer = {'A', 'B', 'C', 'D', 'E'};
            std::vector<size_t> eol_indx;

            f::find_eol_indexes(eol_indx, buffer);

            BOOST_REQUIRE(eol_indx.size() == 0);

            auto line_sptr = std::make_shared< std::vector<std::string> >();
            std::string remainder = "";
            size_t processed = 0; size_t error = 0;
            f::line_from_eol_indexes(line_sptr, remainder, buffer, eol_indx, processed, error);

            BOOST_TEST(line_sptr->size() == 0);
            BOOST_REQUIRE(line_sptr->size() == 0);
            BOOST_TEST(processed == 0);
            BOOST_TEST(error == 0);
            BOOST_TEST(remainder == "ABCDE");
        }

        {
            std::vector<uint8_t> buffer = {'A', 'B', 'C', 'D', '\n'};
            std::vector<size_t> eol_indx;

            f::find_eol_indexes(eol_indx, buffer);

            BOOST_REQUIRE(eol_indx.size() == 1);

            auto line_sptr = std::make_shared< std::vector<std::string> >();
            std::string remainder = "ZZ";
            size_t processed = 0; size_t error = 0;
            f::line_from_eol_indexes(line_sptr, remainder, buffer, eol_indx, processed, error);

            BOOST_TEST(line_sptr->size() == 1);
            BOOST_REQUIRE(line_sptr->size() == 1);
            BOOST_TEST(processed == 1);
            BOOST_TEST(error == 0);
            BOOST_TEST(line_sptr->at(0) == "ZZABCD");
            BOOST_TEST(remainder == "");
        }

        {
            std::vector<uint8_t> buffer = {'\n', 'B', 'C', 'D', 'A'};
            std::vector<size_t> eol_indx;

            f::find_eol_indexes(eol_indx, buffer);

            BOOST_REQUIRE(eol_indx.size() == 1);
            BOOST_TEST(eol_indx[0] == 0);

            auto line_sptr = std::make_shared< std::vector<std::string> >();
            std::string remainder = "ZZ";
            size_t processed = 0; size_t error = 0;
            f::line_from_eol_indexes(line_sptr, remainder, buffer, eol_indx, processed, error);

            BOOST_TEST(line_sptr->size() == 1);
            BOOST_REQUIRE(line_sptr->size() == 1);
            BOOST_TEST(processed == 1);
            BOOST_TEST(error == 0);
            BOOST_TEST(line_sptr->at(0) == "ZZ");
            BOOST_TEST(remainder == "BCDA");
        }

    }


    BOOST_AUTO_TEST_CASE(read_space_separated_file_source_existent_tc) {

        const std::string filecontent = "#! FIELDS aaa bbb.dd\n"
                                  "# OtherHeaderField\n"
                                  "0.112 0.12\n"
                                  "-1.4 0.0e-3\n"
                                  "inf 0.2\n"
                                  "2 nan\n";

        TempTextFile tempFile(filecontent);

        f::read_space_separated_file_source raw_node(tempFile.getName(), 100);

        auto res = std::make_shared<std::vector<std::string>>();
        raw_node(res);

        BOOST_TEST(res->size() == 4);
        BOOST_REQUIRE(res->size() == 4);

        BOOST_TEST(res->at(0) == "0.112 0.12");
        BOOST_TEST(res->at(1) == "-1.4 0.0e-3");
        BOOST_TEST(res->at(2) == "inf 0.2");
        BOOST_TEST(res->at(3) == "2 nan");


    }

    BOOST_AUTO_TEST_CASE(read_space_separated_file_source_nonexistent_tc) {
        BOOST_CHECK_THROW( f::read_space_separated_file_source raw_node("NONEXISTENT", 100);, std::runtime_error );

    }


    BOOST_AUTO_TEST_CASE(read_file_source_normalbuffer_tc) {

        const std::string filecontent = "#! FIELDS aaa bbb.dd\n"
                                        "# OtherHeaderField\n"
                                        "0.112 0.12\n"
                                        "-1.4 0.0e-3\n"
                                        "inf 0.2\n"
                                        "2 nan\n";

        TempTextFile tempFile(filecontent);

        const std::string filecontent_noheader = "0.112 0.12\n"
                                        "-1.4 0.0e-3\n"
                                        "inf 0.2\n"
                                        "2 nan\n";

        TempTextFile tempFile2(filecontent_noheader);



        f::read_file_source raw_node(tempFile.getName(), 100);

        auto res = std::make_shared<std::vector<uint8_t>>();
        raw_node(res);


        BOOST_TEST(buffer_to_string(*res) == buffer_to_string(tempFile2.getRawData()));
        BOOST_TEST(res->size() == tempFile2.getRawData().size());
        BOOST_TEST(*res == tempFile2.getRawData());

    }

    BOOST_AUTO_TEST_CASE(read_file_source_empytlines_tc) {

        const std::string filecontent = "#! FIELDS aaa bbb.dd\n"
                                        "# OtherHeaderField\n"
                                        "0.112 0.11\n"
                                        "\n"
                                        "0.112 0.12\n"
                                        " \n"
                                        "0.112 0.13\n";

        TempTextFile tempFile(filecontent);


        f::read_space_separated_file_source raw_node(tempFile.getName(), 100);

        auto res = std::make_shared<std::vector<std::string>>();
        raw_node(res);

        BOOST_TEST(res->size() == 3);
        BOOST_REQUIRE(res->size() == 3);

        BOOST_TEST(res->at(0) == "0.112 0.11");
        BOOST_TEST(res->at(1) == "0.112 0.12");
        BOOST_TEST(res->at(2) == "0.112 0.13");


    }


    BOOST_AUTO_TEST_CASE(read_space_separated_file_multiblock_tc) {

        std::string header = "#! FIELDS aaa bbb.dd\n"
                             "# OtherHeaderField\n";

        std::stringstream ss;
        ss << header;

        std::vector<std::string> expected_res;
        expected_res.reserve(300);
        size_t expected_size = header.size();
        for(int i = 0; i < 300; i++) {
            std::string s = fmt::format("0.4211 0.{}", i);
            expected_size += s.size() + 1;
            ss << (s + "\n");
            expected_res.push_back(s);
        }

        std::string filecontent = ss.str();

        BOOST_TEST(filecontent.size() == expected_size);

        TempTextFile tempFile(filecontent);

        f::read_space_separated_file_source raw_node(tempFile.getName(), 100);

        std::vector<std::string> res;
        auto local_res = std::make_shared<std::vector<std::string>>();
        bool continue_multiblock = true;
        do {
            local_res->clear();
            continue_multiblock = raw_node(local_res);
            res.insert(res.end(), local_res->begin(), local_res->end());
        } while(continue_multiblock);

        BOOST_TEST(res.size() == expected_res.size());
        BOOST_REQUIRE(res.size() == expected_res.size());

        for(int i = 0; i < expected_res.size(); i++) {
                if(res[i] != expected_res[i]) {
                    BOOST_TEST(res[i] == expected_res[i]);
                    fmt::print("Context: index {}", i );
                    fmt::print("String sizes: res : {} vs expected_res {}\n ", res[i].size(), expected_res[i].size() );
                    size_t max_idx = std::max(res[i].size(), expected_res[i].size() );
                    for(int j = 0; j < max_idx; j ++) {
                        if(j < res[i].size() && j < expected_res[i].size()) {
                            if(res[i][j] != expected_res[i][j]) {
                                fmt::print("at index {} : res : {:#} '{}' vs expected_res {:#} '{}'\n ",
                                        j,
                                        (int) res[i][j],
                                           res[i][j],
                                        (int) expected_res[i][j],
                                           expected_res[i][j]);
                            }
                        } else {
                            if(j < res[i].size()) {
                                fmt::print("at index {} : res : {:#} '{}' vs expected_res <already ended>\n ",
                                           j,
                                           (int) res[i][j],
                                           res[i][j]);
                            }else {
                                // It's j < expected_res[i].size()
                                fmt::print("at index {} : res : <already ended> vs expected_res {:#} '{}'\n ",
                                           j,
                                           (int) expected_res[i][j],
                                           expected_res[i][j]);
                            }
                        }

                    }
                    BOOST_REQUIRE(res[i] == expected_res[i]); // Make it stop at first error
                }

        }

    }

    BOOST_AUTO_TEST_CASE(read_space_separated_file_multiblock_multiblocksize_tc) {

        for(size_t block_size = 5; block_size < 100; block_size++) {
            std::string header = "#! FIELDS aaa bbb.dd\n"
                                 "# OtherHeaderField\n";

            std::stringstream ss;
            ss << header;

            std::vector<std::string> expected_res;
            expected_res.reserve(300);
            size_t expected_size = header.size();
            for(int i = 0; i < 300; i++) {
                std::string s = fmt::format("0.4211 0.{}", i);
                expected_size += s.size() + 1;
                ss << (s + "\n");
                expected_res.push_back(s);
            }

            std::string filecontent = ss.str();

            BOOST_TEST(filecontent.size() == expected_size);

            TempTextFile tempFile(filecontent);

            f::read_space_separated_file_source raw_node(tempFile.getName(), block_size);

            std::vector<std::string> res;
            auto local_res = std::make_shared<std::vector<std::string>>();
            bool ret_val = true;
            do {
                local_res->clear();
                ret_val = raw_node(local_res);
                res.insert(res.end(), local_res->begin(), local_res->end());
            } while(ret_val);

            BOOST_TEST(res.size() == expected_res.size());
            BOOST_REQUIRE(res.size() == expected_res.size());

            for(int i = 0; i < expected_res.size(); i++) {
                    if(res[i] != expected_res[i]) {
                        BOOST_TEST(res[i] == expected_res[i]);
                        fmt::print("Context: block_size {} index {}", block_size, i );
                        fmt::print("String sizes: res : {} vs expected_res {}\n ", res[i].size(), expected_res[i].size() );
                        size_t max_idx = std::max(res[i].size(), expected_res[i].size() );
                        for(int j = 0; j < max_idx; j ++) {
                            if(j < res[i].size() && j < expected_res[i].size()) {
                                if(res[i][j] != expected_res[i][j]) {
                                    fmt::print("at index {} : res : {:#} '{}' vs expected_res {:#} '{}'\n ",
                                               j,
                                               (int) res[i][j],
                                               res[i][j],
                                               (int) expected_res[i][j],
                                               expected_res[i][j]);
                                }
                            } else {
                                if(j < res[i].size()) {
                                    fmt::print("at index {} : res : {:#} '{}' vs expected_res <already ended>\n ",
                                               j,
                                               (int) res[i][j],
                                               res[i][j]);
                                }else {
                                    // It's j < expected_res[i].size()
                                    fmt::print("at index {} : res : <already ended> vs expected_res {:#} '{}'\n ",
                                               j,
                                               (int) expected_res[i][j],
                                               expected_res[i][j]);
                                }
                            }

                        }
                        BOOST_REQUIRE(res[i] == expected_res[i]); // Make it stop at first error
                    }
            }
        }

    }

    BOOST_AUTO_TEST_CASE(read_file_source_noCVdata_tc) {

        const std::string filecontent = "#! FIELDS aaa bbb.dd\n"
                                        "# OtherHeaderField";

        TempTextFile tempFile(filecontent);


        BOOST_CHECK_THROW(f::read_file_source raw_node(tempFile.getName(), 5);, std::runtime_error );


    }

BOOST_AUTO_TEST_SUITE_END();