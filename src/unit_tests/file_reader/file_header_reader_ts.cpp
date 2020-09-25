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
#include "../TempTextFile.hpp"

namespace f = fesutils;

BOOST_AUTO_TEST_SUITE(file_header_reader_ts)

    BOOST_AUTO_TEST_CASE(read_cv_file_header_nonexistent_tc) {

        BOOST_CHECK_THROW( f::read_cv_file_header("NONEXISTENT.file"), std::runtime_error );

    }

    BOOST_AUTO_TEST_CASE(read_cv_file_header_existent_tc) {

        const std::string filecontent = "#! FIELDS aaa bbb.dd\n"
                                        "#! SET min_aaa 4.90\n"
                                        "#! SET normalisation 2.20\n"
                                        "# OtherHeaderField\n"
                                        "0.112 0.12\n"
                                        "-1.4 0.0e-3\n"
                                        "inf 0.2\n"
                                        "2 nan\n";

        TempTextFile tempFile(filecontent);

        f::PlumedDatHeader header = f::read_cv_file_header(tempFile.getName());

        BOOST_TEST(header.fields.size() == 2);
        BOOST_REQUIRE(header.fields.size() == 2);

        BOOST_TEST(header.fields[0].name == "aaa");
        BOOST_TEST(header.fields[1].name == "bbb.dd");

        BOOST_TEST(header.fields[0].is_subcomponent == false);
        BOOST_TEST(header.fields[1].is_subcomponent == true);

        BOOST_TEST(header.fields[1].basename == "bbb");
        BOOST_TEST(header.fields[1].subfield == "dd");

        BOOST_TEST(header.attributes.size() == 1);
        BOOST_REQUIRE(header.attributes.size() == 1);
        BOOST_TEST(header.attributes.count("normalisation") == 1);
        BOOST_REQUIRE(header.attributes.count("normalisation") == 1);
        BOOST_TEST(std::holds_alternative<double>(header.attributes["normalisation"]));
        BOOST_REQUIRE(std::holds_alternative<double>(header.attributes["normalisation"]));
        BOOST_TEST(std::get<double>(header.attributes["normalisation"]) == 2.20);


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

    BOOST_AUTO_TEST_CASE(parse_SET_line_tc) {

        {
            f::PlumedDatHeader header;
            f::parse_FIELDS_line(header, "#! FIELDS a b.d c..d");

            f::parse_SET_line(header, "#! SET min_a 2.4");

            BOOST_TEST(header.fields.size() == 3);
            BOOST_REQUIRE(header.fields.size() == 3);

            BOOST_REQUIRE(header.fields[0].name == "a");

            BOOST_TEST(header.fields[0].attributes.size() == 1);
            BOOST_REQUIRE(header.fields[0].attributes.size() == 1);
            BOOST_REQUIRE(std::holds_alternative<double>(header.fields[0].attributes["min"]));
            BOOST_TEST(std::get<double>(header.fields[0].attributes["min"]) == 2.4);

            BOOST_CHECK_THROW( std::get<bool>(header.fields[0].attributes["min"]), std::bad_variant_access );
            BOOST_CHECK_THROW( std::get<double>(header.fields[0].attributes["map"]), std::bad_variant_access );
        }

        {
            f::PlumedDatHeader header;
            f::parse_FIELDS_line(header, "#! FIELDS a b.d c..d");

            f::parse_SET_line(header, "#! SET normalisation 2.4");

            BOOST_TEST(header.attributes.size() == 1);
            BOOST_REQUIRE(header.attributes.size() == 1);
            BOOST_TEST(header.attributes.count("normalisation") == 1);
            BOOST_REQUIRE(header.attributes.count("normalisation") == 1);
            BOOST_TEST(std::holds_alternative<double>(header.attributes["normalisation"]));
            BOOST_REQUIRE(std::holds_alternative<double>(header.attributes["normalisation"]));
            BOOST_TEST(std::get<double>(header.attributes["normalisation"]) == 2.4);


            BOOST_CHECK_THROW( std::get<bool>(header.attributes["normalisation"]), std::bad_variant_access );
            BOOST_CHECK_THROW( std::get<int>(header.attributes["normalisation"]), std::bad_variant_access );

            f::parse_SET_line(header, "#! SET globalattributenotknown 2.4");

            // Should not be added if not known
            BOOST_TEST(header.attributes.size() == 1);

        }

        {
            f::PlumedDatHeader header;
            f::parse_FIELDS_line(header, "#! FIELDS a b.d c..d");

            // Unknown attribute should not in be added to the map
            f::parse_SET_line(header, "#! SET moop_a 2.4");


            BOOST_REQUIRE(header.fields[0].name == "a");

            BOOST_TEST(header.fields[0].attributes.size() == 0);

            // Testing int attribute
            f::parse_SET_line(header, "#! SET nbins_a 523");

            BOOST_TEST(header.fields[0].attributes.size() == 1);
            BOOST_REQUIRE(header.fields[0].attributes.size() == 1);
            BOOST_REQUIRE(std::holds_alternative<int>(header.fields[0].attributes["nbins"]));
            BOOST_TEST(std::get<int>(header.fields[0].attributes["nbins"]) == 523);

            // Testing bool attribute
            f::parse_SET_line(header, "#! SET periodic_a false");

            BOOST_TEST(header.fields[0].attributes.size() == 2);
            BOOST_REQUIRE(header.fields[0].attributes.size() == 2);
            BOOST_REQUIRE(std::holds_alternative<bool>(header.fields[0].attributes["periodic"]));
            BOOST_TEST(std::get<bool>(header.fields[0].attributes["periodic"]) == false);

        }

        {
            f::PlumedDatHeader header;
            f::parse_FIELDS_line(header, "#! FIELDS a b.d c..d");

            // Malformed line: too short
            BOOST_CHECK_THROW(f::parse_SET_line(header, "#! SET min_a"), std::runtime_error );

            // Should be double, but is bool: should throw
            BOOST_CHECK_THROW(f::parse_SET_line(header, "#! SET min_a false"), std::runtime_error );

            // Should be int, but is double: should throw
            BOOST_CHECK_THROW(f::parse_SET_line(header, "#! SET nbins_a 2.8"), std::runtime_error );

            // Should be bool, but is double: should throw
            BOOST_CHECK_THROW(f::parse_SET_line(header, "#! SET periodic_a 2.8"), std::runtime_error );

            // Should be double, but is bool: should throw
            BOOST_CHECK_THROW(f::parse_SET_line(header, "#! SET normalisation false"), std::runtime_error );

        }

    }

    BOOST_AUTO_TEST_CASE(extract_attribute_field_from_attrfield_tc) {

        {
            std::tuple<std::string, std::string> res = f::extract_attribute_field_from_attrfield("attri_varname");

            BOOST_TEST(std::get<0>(res) == "attri");
            BOOST_TEST(std::get<1>(res) == "varname");
        }

        {
            std::tuple<std::string, std::string> res = f::extract_attribute_field_from_attrfield("nounderscore");

            BOOST_TEST(std::get<0>(res) == "nounderscore");
            BOOST_TEST(std::get<1>(res) == "");
        }

        BOOST_CHECK_THROW( f::extract_attribute_field_from_attrfield("emptyvarname_"), std::runtime_error );
        BOOST_CHECK_THROW( f::extract_attribute_field_from_attrfield("_emptyattribute"), std::runtime_error );

    }


    BOOST_AUTO_TEST_CASE(remove_nongrid_field_tc) {

        const std::string filecontent = "#! FIELDS aaa bbb\n"
                                        "#! SET min_aaa 4.90\n"
                                        "#! SET max_aaa 8.90\n"
                                        "#! SET nbins_aaa 10\n"
                                        "#! SET periodic_aaa false\n"
                                        "#! SET normalisation 2.20\n"
                                        "# OtherHeaderField\n"
                                        "0.112 0.12\n"
                                        "-1.4 0.0e-3\n"
                                        "inf 0.2\n"
                                        "2 nan\n";

        TempTextFile tempFile(filecontent);

        f::PlumedDatHeader header = f::read_cv_file_header(tempFile.getName());

        BOOST_TEST(header.fields.size() == 2);
        BOOST_REQUIRE(header.fields.size() == 2);

        remove_nongrid_fields(header);


        BOOST_TEST(header.fields.size() == 1);

    }


BOOST_AUTO_TEST_SUITE_END();