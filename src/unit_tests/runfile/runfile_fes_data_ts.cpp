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
#include "../../runfile/runfile_fes_data.hpp"
#include "../../GeneralOptions.h"
#include "../../runfile/runfile_parsed_tree.hpp"
#include "../TempDirectory.hpp"

namespace {
    const std::string yaml_fesdata_correct = "meac:\n"
                                                    "    name: \"Methylacetate\"\n"
                                                    "    cv_varnames:\n"
                                                    "        - \"fragment_position.x\"\n"
                                                    "        - \"fragment_position.y\"\n"
                                                    "        - \"fragment_position.z\"\n"
                                                    "    wall_bias_varnames:\n"
                                                    "        - \"wall_lower.bias\"\n"
                                                    "        - \"wall_upper.bias\"\n"
                                                    "    metad_bias_varname: \"metad_position.bias\"\n"
                                                    "    cv_filename_hint: \"CV_fragment_meac\"\n"
                                                    "    hills_filename_hint: \"HILLS_fragment_meac\"";
    const std::string fesdata_name_reference = "Methylacetate";
    const std::set<std::string> fesdata_cv_varnames_reference = {"fragment_position.x",
                                                                 "fragment_position.y",
                                                                 "fragment_position.z"};

    const std::set<std::string> fesdata_wallbias_varnames_reference = {"wall_lower.bias",
                                                                        "wall_upper.bias"};

    const std::string fesdata_name_metad_varname_reference = "metad_position.bias";
    const std::string fesdata_cv_filehint_reference = "CV_fragment_meac";
    const std::string fesdata_hills_filehint_reference = "HILLS_fragment_meac";


    const std::string yaml_fesdata_missing_name = "meac:\n"
                                                    "    cv_varnames:\n"
                                                    "        - \"fragment_position.x\"\n"
                                                    "        - \"fragment_position.y\"\n"
                                                    "        - \"fragment_position.z\"\n"
                                                    "    wall_bias_varnames:\n"
                                                    "        - \"wall_lower.bias\"\n"
                                                    "        - \"wall_upper.bias\"\n"
                                                    "    metad_bias_varname: \"metad_position.bias\"\n"
                                                    "    cv_filename_hint: \"CV_fragment_meac\"\n"
                                                    "    hills_filename_hint: \"HILLS_fragment_meac\"";

    const std::string yaml_fesdata_missing_name_with_grid = "meac:\n"
                                                    "    name: \"Methylacetate\"\n"
                                                    "    cv_varnames:\n"
                                                    "        - \"fragment_position.x\"\n"
                                                    "        - \"fragment_position.y\"\n"
                                                    "        - \"fragment_position.z\"\n"
                                                    "    wall_bias_varnames:\n"
                                                    "        - \"wall_lower.bias\"\n"
                                                    "        - \"wall_upper.bias\"\n"
                                                    "    metad_bias_varname: \"metad_position.bias\"\n"
                                                    "    cv_filename_hint: \"CV_fragment_meac\"\n"
                                                    "    grid_filename_hint: \"GRID_fragment_meac\"";
    const std::string yaml_fesdata_missing_cv_filename = "meac:\n"
                                                    "    name: \"Methylacetate\"\n"
                                                    "    cv_varnames:\n"
                                                    "        - \"fragment_position.x\"\n"
                                                    "        - \"fragment_position.y\"\n"
                                                    "        - \"fragment_position.z\"\n"
                                                    "    wall_bias_varnames:\n"
                                                    "        - \"wall_lower.bias\"\n"
                                                    "        - \"wall_upper.bias\"\n"
                                                    "    metad_bias_varname: \"metad_position.bias\"\n"
                                                    "    hills_filename_hint: \"HILLS_fragment_meac\"";
    const std::string yaml_fesdata_missing_hills_and_grid= "meac:\n"
                                             "    name: \"Methylacetate\"\n"
                                             "    cv_varnames:\n"
                                             "        - \"fragment_position.x\"\n"
                                             "        - \"fragment_position.y\"\n"
                                             "        - \"fragment_position.z\"\n"
                                             "    wall_bias_varnames:\n"
                                             "        - \"wall_lower.bias\"\n"
                                             "        - \"wall_upper.bias\"\n"
                                             "    metad_bias_varname: \"metad_position.bias\"\n"
                                             "    cv_filename_hint: \"CV_fragment_meac\"\n";
    const std::string yaml_fesdata_missing_cv_varnames = "meac:\n"
                                             "    name: \"Methylacetate\"\n"
                                             "    wall_bias_varnames:\n"
                                             "        - \"wall_lower.bias\"\n"
                                             "        - \"wall_upper.bias\"\n"
                                             "    metad_bias_varname: \"metad_position.bias\"\n"
                                             "    cv_filename_hint: \"CV_fragment_meac\"\n"
                                             "    hills_filename_hint: \"HILLS_fragment_meac\"";

    const std::string yaml_fesdata_cv_varname_incorrect = "meac:\n"
                                             "    name: \"Methylacetate\"\n"
                                             "    cv_varnames: 1\n"
                                             "    wall_bias_varnames:\n"
                                             "        - \"wall_lower.bias\"\n"
                                             "        - \"wall_upper.bias\"\n"
                                             "    metad_bias_varname: \"metad_position.bias\"\n"
                                             "    cv_filename_hint: \"CV_fragment_meac\"\n"
                                             "    hills_filename_hint: \"HILLS_fragment_meac\"";
    const std::string yaml_fesdata_missing_metad_varname = "meac:\n"
                                             "    name: \"Methylacetate\"\n"
                                             "    cv_varnames:\n"
                                             "        - \"fragment_position.x\"\n"
                                             "        - \"fragment_position.y\"\n"
                                             "        - \"fragment_position.z\"\n"
                                             "    wall_bias_varnames:\n"
                                             "        - \"wall_lower.bias\"\n"
                                             "        - \"wall_upper.bias\"\n"
                                             "    cv_filename_hint: \"CV_fragment_meac\"\n"
                                             "    hills_filename_hint: \"HILLS_fragment_meac\"";
    const std::string yaml_fesdata_nonscalar_metad_varname = "meac:\n"
                                                           "    name: \"Methylacetate\"\n"
                                                           "    cv_varnames:\n"
                                                           "        - \"fragment_position.x\"\n"
                                                           "        - \"fragment_position.y\"\n"
                                                           "        - \"fragment_position.z\"\n"
                                                           "    wall_bias_varnames:\n"
                                                           "        - \"wall_lower.bias\"\n"
                                                           "        - \"wall_upper.bias\"\n"
                                                           "    metad_bias_varname:\n"
                                                           "        - \"metad_position.bias\"\n"
                                                           "        - \"metad_position2.bias\"\n"
                                                           "    cv_filename_hint: \"CV_fragment_meac\"\n"
                                                           "    hills_filename_hint: \"HILLS_fragment_meac\"";

}

namespace f = fesutils;

BOOST_AUTO_TEST_SUITE(runfilme_fes_data_ts)

    BOOST_AUTO_TEST_CASE(correct_block_tc, *boost::unit_test::tolerance(0.001)) {

        // PRELUDE
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("CV_fragment_meac",
                                                                               "NOCONTENTNECESSARY");
        bf::path grid_file_path = temp_file->getFilePath();

        std::shared_ptr<TempTextFile>  temp_bindump = td.createTemporaryFileInDir("HILLS_fragment_meac",
                                                                                  "NOCONTENTNECESSARY");
        bf::path bindump_path = temp_bindump->getFilePath();

        f::GeneralOptions options;

        options.search_directory["all"] = td_path.generic_string();


        // TEST

        YAML::Node fes_data_node = YAML::Load(yaml_fesdata_correct);

        f::parse_fes_data_section(options, fes_data_node);

        BOOST_TEST(f::g_runfileitem_fesdata_list.count("meac") == 1);
        BOOST_REQUIRE(f::g_runfileitem_fesdata_list.count("meac") == 1);

        const f::RunfileItem_FESData item = f::g_runfileitem_fesdata_list["meac"];
        f::g_runfileitem_fesdata_list.clear();

        BOOST_TEST(item.name == fesdata_name_reference);

        std::set<std::string> fesdata_actual_varnames;
        for(const std::string& cv_varname: item.cv_varnames) {
            fesdata_actual_varnames.insert(cv_varname);
        }

        BOOST_TEST(fesdata_cv_varnames_reference == fesdata_actual_varnames);

        std::set<std::string> fesdata_actual_wallbiasnames;
        for(const std::string& wall_bias_varname: item.wall_bias_varnames) {
            fesdata_actual_wallbiasnames.insert(wall_bias_varname);
        }

        BOOST_TEST(fesdata_wallbias_varnames_reference == fesdata_actual_wallbiasnames);

        BOOST_TEST(fesdata_name_metad_varname_reference == item.metad_bias_varname);

        BOOST_TEST(fesdata_cv_filehint_reference == item.cv_filename_hint);
        bf::path cv_path = td_path;
        cv_path /= fesdata_cv_filehint_reference;
        BOOST_TEST(cv_path == item.cv_file_path);


        BOOST_TEST(fesdata_hills_filehint_reference == item.hills_filename_hint);
        bf::path hills_path = td_path;
        hills_path /= fesdata_hills_filehint_reference;
        BOOST_REQUIRE(item.hills_file_path.has_value());
        BOOST_TEST(hills_path == item.hills_file_path.value());

        BOOST_REQUIRE(!item.grid_file_path.has_value());


    }

    BOOST_AUTO_TEST_CASE(missing_name_tc, *boost::unit_test::tolerance(0.001)) {

        // PRELUDE
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("CV_fragment_meac",
                                                                               "NOCONTENTNECESSARY");
        bf::path grid_file_path = temp_file->getFilePath();

        std::shared_ptr<TempTextFile>  temp_bindump = td.createTemporaryFileInDir("HILLS_fragment_meac",
                                                                                  "NOCONTENTNECESSARY");
        bf::path bindump_path = temp_bindump->getFilePath();

        f::GeneralOptions options;

        options.search_directory["all"] = td_path.generic_string();


        // TEST

        YAML::Node fes_data_node = YAML::Load(yaml_fesdata_missing_name);

        BOOST_CHECK_THROW(f::parse_fes_data_section(options, fes_data_node), std::runtime_error);

        BOOST_TEST(f::g_runfileitem_fesdata_list.count("meac") == 0);

    }

    BOOST_AUTO_TEST_CASE(not_a_map_tc, *boost::unit_test::tolerance(0.001)) {

        // PRELUDE
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("CV_fragment_meac",
                                                                               "NOCONTENTNECESSARY");
        bf::path grid_file_path = temp_file->getFilePath();

        std::shared_ptr<TempTextFile>  temp_bindump = td.createTemporaryFileInDir("HILLS_fragment_meac",
                                                                                  "NOCONTENTNECESSARY");
        bf::path bindump_path = temp_bindump->getFilePath();

        f::GeneralOptions options;

        options.search_directory["all"] = td_path.generic_string();


        // TEST

        YAML::Node fes_data_node = YAML::Load("[1, 2, 3]"); // This is a sequence and not a map

        BOOST_CHECK_THROW(f::parse_fes_data_section(options, fes_data_node), std::runtime_error);

        BOOST_TEST(f::g_runfileitem_fesdata_list.count("meac") == 0);

    }

    BOOST_AUTO_TEST_CASE(not_a_map_of_map_tc, *boost::unit_test::tolerance(0.001)) {

        // PRELUDE
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("CV_fragment_meac",
                                                                               "NOCONTENTNECESSARY");
        bf::path grid_file_path = temp_file->getFilePath();

        std::shared_ptr<TempTextFile>  temp_bindump = td.createTemporaryFileInDir("HILLS_fragment_meac",
                                                                                  "NOCONTENTNECESSARY");
        bf::path bindump_path = temp_bindump->getFilePath();

        f::GeneralOptions options;

        options.search_directory["all"] = td_path.generic_string();


        // TEST

        YAML::Node fes_data_node = YAML::Load("a: [1, 2, 3]"); // Map of sequence: incorrect

        BOOST_CHECK_THROW(f::parse_fes_data_section(options, fes_data_node), std::runtime_error);

        BOOST_TEST(f::g_runfileitem_fesdata_list.count("meac") == 0);

    }

    BOOST_AUTO_TEST_CASE(with_grid_tc, *boost::unit_test::tolerance(0.001)) {

        // PRELUDE
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("CV_fragment_meac",
                                                                               "NOCONTENTNECESSARY");
        bf::path grid_file_path = temp_file->getFilePath();

        std::shared_ptr<TempTextFile>  temp_bindump = td.createTemporaryFileInDir("GRID_fragment_meac",
                                                                                  "NOCONTENTNECESSARY");
        bf::path bindump_path = temp_bindump->getFilePath();

        f::GeneralOptions options;

        options.search_directory["all"] = td_path.generic_string();


        // TEST

        YAML::Node fes_data_node = YAML::Load(yaml_fesdata_missing_name_with_grid); // This is a sequence and not a map

        f::parse_fes_data_section(options, fes_data_node);

        BOOST_TEST(f::g_runfileitem_fesdata_list.count("meac") == 1);
        BOOST_REQUIRE(f::g_runfileitem_fesdata_list.count("meac") == 1);

        const f::RunfileItem_FESData item = f::g_runfileitem_fesdata_list["meac"];
        f::g_runfileitem_fesdata_list.clear();

        BOOST_TEST(item.grid_filename_hint == "GRID_fragment_meac");
        bf::path grid_path = td_path;
        grid_path /= "GRID_fragment_meac";
        BOOST_REQUIRE(item.grid_file_path.has_value());
        BOOST_TEST(grid_path == item.grid_file_path.value());

    }

    BOOST_AUTO_TEST_CASE(missing_cv_filename_tc, *boost::unit_test::tolerance(0.001)) {

        // PRELUDE
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("CV_fragment_meac",
                                                                               "NOCONTENTNECESSARY");
        bf::path grid_file_path = temp_file->getFilePath();

        std::shared_ptr<TempTextFile>  temp_bindump = td.createTemporaryFileInDir("HILLS_fragment_meac",
                                                                                  "NOCONTENTNECESSARY");
        bf::path bindump_path = temp_bindump->getFilePath();

        f::GeneralOptions options;

        options.search_directory["all"] = td_path.generic_string();


        // TEST

        YAML::Node fes_data_node = YAML::Load(yaml_fesdata_missing_cv_filename);

        BOOST_CHECK_THROW(f::parse_fes_data_section(options, fes_data_node), std::runtime_error);

        BOOST_TEST(f::g_runfileitem_fesdata_list.count("meac") == 0);

    }

    BOOST_AUTO_TEST_CASE(missing_hills_and_grid_tc, *boost::unit_test::tolerance(0.001)) {

        // PRELUDE
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("CV_fragment_meac",
                                                                               "NOCONTENTNECESSARY");
        bf::path grid_file_path = temp_file->getFilePath();

        std::shared_ptr<TempTextFile>  temp_bindump = td.createTemporaryFileInDir("HILLS_fragment_meac",
                                                                                  "NOCONTENTNECESSARY");
        bf::path bindump_path = temp_bindump->getFilePath();

        f::GeneralOptions options;

        options.search_directory["all"] = td_path.generic_string();


        // TEST

        YAML::Node fes_data_node = YAML::Load(yaml_fesdata_missing_hills_and_grid);

        BOOST_CHECK_THROW(f::parse_fes_data_section(options, fes_data_node), std::runtime_error);

        BOOST_TEST(f::g_runfileitem_fesdata_list.count("meac") == 0);

    }

    BOOST_AUTO_TEST_CASE(missing_cv_varnames_tc, *boost::unit_test::tolerance(0.001)) {

        // PRELUDE
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("CV_fragment_meac",
                                                                               "NOCONTENTNECESSARY");
        bf::path grid_file_path = temp_file->getFilePath();

        std::shared_ptr<TempTextFile>  temp_bindump = td.createTemporaryFileInDir("HILLS_fragment_meac",
                                                                                  "NOCONTENTNECESSARY");
        bf::path bindump_path = temp_bindump->getFilePath();

        f::GeneralOptions options;

        options.search_directory["all"] = td_path.generic_string();


        // TEST

        YAML::Node fes_data_node = YAML::Load(yaml_fesdata_missing_cv_varnames);

        BOOST_CHECK_THROW(f::parse_fes_data_section(options, fes_data_node), std::runtime_error);

        BOOST_TEST(f::g_runfileitem_fesdata_list.count("meac") == 0);

    }

    BOOST_AUTO_TEST_CASE(incorrect_cv_varnames_tc, *boost::unit_test::tolerance(0.001)) {

        // PRELUDE
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("CV_fragment_meac",
                                                                               "NOCONTENTNECESSARY");
        bf::path grid_file_path = temp_file->getFilePath();

        std::shared_ptr<TempTextFile>  temp_bindump = td.createTemporaryFileInDir("HILLS_fragment_meac",
                                                                                  "NOCONTENTNECESSARY");
        bf::path bindump_path = temp_bindump->getFilePath();

        f::GeneralOptions options;

        options.search_directory["all"] = td_path.generic_string();


        // TEST

        YAML::Node fes_data_node = YAML::Load(yaml_fesdata_cv_varname_incorrect);

        BOOST_CHECK_THROW(f::parse_fes_data_section(options, fes_data_node), std::runtime_error);

        BOOST_TEST(f::g_runfileitem_fesdata_list.count("meac") == 0);

    }

    BOOST_AUTO_TEST_CASE(missing_metad_bias_varname_tc, *boost::unit_test::tolerance(0.001)) {

        // PRELUDE
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("CV_fragment_meac",
                                                                               "NOCONTENTNECESSARY");
        bf::path grid_file_path = temp_file->getFilePath();

        std::shared_ptr<TempTextFile>  temp_bindump = td.createTemporaryFileInDir("HILLS_fragment_meac",
                                                                                  "NOCONTENTNECESSARY");
        bf::path bindump_path = temp_bindump->getFilePath();

        f::GeneralOptions options;

        options.search_directory["all"] = td_path.generic_string();


        // TEST

        YAML::Node fes_data_node = YAML::Load(yaml_fesdata_missing_metad_varname);

        BOOST_CHECK_THROW(f::parse_fes_data_section(options, fes_data_node), std::runtime_error);

        BOOST_TEST(f::g_runfileitem_fesdata_list.count("meac") == 0);

    }

    BOOST_AUTO_TEST_CASE(nonscalar_metad_bias_varname_tc, *boost::unit_test::tolerance(0.001)) {

        // PRELUDE
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("CV_fragment_meac",
                                                                               "NOCONTENTNECESSARY");
        bf::path grid_file_path = temp_file->getFilePath();

        std::shared_ptr<TempTextFile>  temp_bindump = td.createTemporaryFileInDir("HILLS_fragment_meac",
                                                                                  "NOCONTENTNECESSARY");
        bf::path bindump_path = temp_bindump->getFilePath();

        f::GeneralOptions options;

        options.search_directory["all"] = td_path.generic_string();


        // TEST

        YAML::Node fes_data_node = YAML::Load(yaml_fesdata_nonscalar_metad_varname);

        BOOST_CHECK_THROW(f::parse_fes_data_section(options, fes_data_node), std::runtime_error);

        BOOST_TEST(f::g_runfileitem_fesdata_list.count("meac") == 0);

    }




BOOST_AUTO_TEST_SUITE_END();