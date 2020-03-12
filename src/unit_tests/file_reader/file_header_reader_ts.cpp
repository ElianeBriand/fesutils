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

#include "../../file_reader/read_file_header.hpp"

namespace f = fesutils;

BOOST_AUTO_TEST_SUITE(file_header_reader_ts)

    BOOST_AUTO_TEST_CASE(read_cv_file_header_nonexistent_tc) {

        BOOST_CHECK_THROW( f::read_cv_file_header("NONEXISTENT.file"), std::runtime_error );

    }

    BOOST_AUTO_TEST_CASE(read_cv_file_header_existent_tc) {



        #ifdef __linux__
                char namebuf[strlen("test.XXXXXX") + 1];
                std::strcpy(namebuf,"test.XXXXXX");
                std::FILE* tmpf = fdopen(mkstemp(namebuf), "wb");
                std::string name1 = namebuf;
        #else
                std::string name1 = std::tmpnam(nullptr);
                std::FILE* tmpf = std::fopen(name1.c_str(), "wb");
        #endif

        BOOST_REQUIRE(tmpf != nullptr);

        std::fputs("#! FIELDS aaa bbb.dd\n"
                   "# OtherHeaderField\n"
                   "0.112 0.12\n"
                   "-1.4 0.0e-3\n"
                   "inf 0.2\n"
                   "2 nan\n", tmpf);

        std::fflush(tmpf);

        f::PlumedDatHeader header = f::read_cv_file_header(name1);

        BOOST_TEST(header.fields.size() == 2);
        BOOST_REQUIRE(header.fields.size() == 2);

        BOOST_TEST(header.fields[0].name == "aaa");
        BOOST_TEST(header.fields[1].name == "bbb.dd");

        BOOST_TEST(header.fields[0].is_subcomponent == false);
        BOOST_TEST(header.fields[1].is_subcomponent == true);

        BOOST_TEST(header.fields[1].basename == "bbb");
        BOOST_TEST(header.fields[1].subfield == "dd");

        std::fclose(tmpf);
        std::remove(name1.c_str());

    }

    BOOST_AUTO_TEST_CASE(extract_basename_from_field_name_tc) {

        {
            auto [basename, subfield] = f::extract_basename_subfield_from_field_name("basename.subfield");

            BOOST_TEST(basename == "basename");
            BOOST_TEST(subfield == "subfield");
        }

        {
            auto [basename, subfield] = f::extract_basename_subfield_from_field_name("nosubfield");

            BOOST_TEST(basename == "nosubfield");
            BOOST_TEST(subfield == "");
        }

        {
            auto [basename, subfield] = f::extract_basename_subfield_from_field_name("doesnot..followtherule");

            BOOST_TEST(basename == "doesnot..followtherule");
            BOOST_TEST(subfield == "");
        }

        {
            auto [basename, subfield] = f::extract_basename_subfield_from_field_name("doesnot.");

            BOOST_TEST(basename == "doesnot.");
            BOOST_TEST(subfield == "");
        }


    }

    BOOST_AUTO_TEST_CASE(parse_FIELDS_line_tc) {
        f::PlumedDatHeader header;
        f::parse_FIELDS_line(header, "#! FIELDS a b.d c..d");

        for(auto& e: header.fields) {
            std::cout << e.name << std::endl;
        }

        BOOST_TEST(header.fields.size() == 3);
        BOOST_REQUIRE(header.fields.size() == 3);

        BOOST_TEST(header.fields[0].name == "a");
        BOOST_TEST(header.fields[1].name == "b.d");
        BOOST_TEST(header.fields[2].name == "c..d");

        BOOST_TEST(header.fields[0].is_subcomponent == false);
        BOOST_TEST(header.fields[1].is_subcomponent == true);
        BOOST_TEST(header.fields[2].is_subcomponent == false);

        BOOST_TEST(header.fields[1].basename == "b");
        BOOST_TEST(header.fields[1].subfield == "d");

        BOOST_CHECK_THROW( f::parse_FIELDS_line(header, "#! FIELDS "), std::runtime_error );
        BOOST_CHECK_THROW( f::parse_FIELDS_line(header, "#! FIELDS     "), std::runtime_error );

    }


BOOST_AUTO_TEST_SUITE_END();