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
#include "../TempDirectory.hpp"
#include "../../GeneralOptions.h"
#include "../../runfile/yaml_frontend.hpp"
#include "../../runfile/runfile_parsed_tree.hpp"


namespace f = fesutils;

namespace {
    const std::string yaml_correct_runfile_template = "general:\n"
                                                      "    title: \"Reweight & fragment reconstruction\"\n"
                                                      "    \n"
                                                      "search_directory:\n"
                                                      "    all: \"{:s}\"\n"
                                                      "    \n"
                                                      "reweight:\n"
                                                      "    rw_all:\n"
                                                      "        method: reweight_no_interp\n"
                                                      "        wall_reached: discard\n"
                                                      "        data: [meac, bzn, pip]\n"
                                                      "\n"
                                                      "histogram:\n"
                                                      "    histo_all:\n"
                                                      "        method: histo_no_interp\n"
                                                      "        data: [meac, bzn, pip]\n"
                                                      "        \n"
                                                      "        \n"
                                                      "reconstruction:\n"
                                                      "    mph_1point:\n"
                                                      "        name: \"Methylphenidate (single point reconstruction)\"\n"
                                                      "        fragments: [meac, bzn, pip]\n"
                                                      "        method: single_point \n"
                                                      "fes_data:\n"
                                                      "    meac:\n"
                                                      "        name: \"Methylacetate\"\n"
                                                      "        cv_varnames:\n"
                                                      "            - \"fragment_position.x\"\n"
                                                      "            - \"fragment_position.y\"\n"
                                                      "            - \"fragment_position.z\"\n"
                                                      "        wall_bias_varnames:\n"
                                                      "            - \"wall_lower.bias\"\n"
                                                      "            - \"wall_upper.bias\"\n"
                                                      "        metad_bias_varname: \"metad_position.bias\"\n"
                                                      "        cv_filename_hint: \"CV_fragment_meac\"\n"
                                                      "        hills_filename_hint: \"HILLS_fragment_meac\"\n"
                                                      "        \n"
                                                      "steps:\n"
                                                      "    - fes_data : [meac]";

    const std::string yaml_runfile_template_missing_general =
                                                      "search_directory:\n"
                                                      "    all: \"{:s}\"\n"
                                                      "    \n"
                                                      "fes_data:\n"
                                                      "    meac:\n"
                                                      "        name: \"Methylacetate\"\n"
                                                      "        cv_varnames:\n"
                                                      "            - \"fragment_position.x\"\n"
                                                      "            - \"fragment_position.y\"\n"
                                                      "            - \"fragment_position.z\"\n"
                                                      "        wall_bias_varnames:\n"
                                                      "            - \"wall_lower.bias\"\n"
                                                      "            - \"wall_upper.bias\"\n"
                                                      "        metad_bias_varname: \"metad_position.bias\"\n"
                                                      "        cv_filename_hint: \"CV_fragment_meac\"\n"
                                                      "        hills_filename_hint: \"HILLS_fragment_meac\"\n"
                                                      "        \n"
                                                      "steps:\n"
                                                      "    - fes_data : [meac]";
    const std::string yaml_runfile_template_missing_search_dir = "general:\n"
                                                      "    title: \"Reweight & fragment reconstruction\"\n"
                                                      "    \n"
                                                      "    \n"
                                                      "fes_data:\n"
                                                      "    meac:\n"
                                                      "        name: \"Methylacetate\"\n"
                                                      "        cv_varnames:\n"
                                                      "            - \"fragment_position.x\"\n"
                                                      "            - \"fragment_position.y\"\n"
                                                      "            - \"fragment_position.z\"\n"
                                                      "        wall_bias_varnames:\n"
                                                      "            - \"wall_lower.bias\"\n"
                                                      "            - \"wall_upper.bias\"\n"
                                                      "        metad_bias_varname: \"metad_position.bias\"\n"
                                                      "        cv_filename_hint: \"CV_fragment_meac\"\n"
                                                      "        hills_filename_hint: \"HILLS_fragment_meac\"\n"
                                                      "        \n"
                                                      "steps:\n"
                                                      "    - fes_data : [meac]";

    const std::string yaml_correct_runfile_template_missing_fes_data = "general:\n"
                                                      "    title: \"Reweight & fragment reconstruction\"\n"
                                                      "    \n"
                                                      "search_directory:\n"
                                                      "    all: \"{:s}\"\n"
                                                      "    \n"
                                                      "reweight:\n"
                                                      "    rw_all:\n"
                                                      "        method: reweight_no_interp\n"
                                                      "        wall_reached: discard\n"
                                                      "        data: [meac, bzn, pip]\n"
                                                      "\n"
                                                      "histogram:\n"
                                                      "    histo_all:\n"
                                                      "        method: histo_no_interp\n"
                                                      "        data: [meac, bzn, pip]\n"
                                                      "        \n"
                                                      "        \n"
                                                      "reconstruction:\n"
                                                      "    mph_1point:\n"
                                                      "        name: \"Methylphenidate (single point reconstruction)\"\n"
                                                      "        fragments: [meac, bzn, pip]\n"
                                                      "        method: single_point \n"
                                                      "steps:\n"
                                                      "    - fes_data : [meac]";
    const std::string yaml_correct_runfile_template_missing_steps = "general:\n"
                                                      "    title: \"Reweight & fragment reconstruction\"\n"
                                                      "    \n"
                                                      "search_directory:\n"
                                                      "    all: \"{:s}\"\n"
                                                      "    \n"
                                                      "reweight:\n"
                                                      "    rw_all:\n"
                                                      "        method: reweight_no_interp\n"
                                                      "        wall_reached: discard\n"
                                                      "        data: [meac, bzn, pip]\n"
                                                      "\n"
                                                      "histogram:\n"
                                                      "    histo_all:\n"
                                                      "        method: histo_no_interp\n"
                                                      "        data: [meac, bzn, pip]\n"
                                                      "        \n"
                                                      "        \n"
                                                      "reconstruction:\n"
                                                      "    mph_1point:\n"
                                                      "        name: \"Methylphenidate (single point reconstruction)\"\n"
                                                      "        fragments: [meac, bzn, pip]\n"
                                                      "        method: single_point \n"
                                                      "fes_data:\n"
                                                      "    meac:\n"
                                                      "        name: \"Methylacetate\"\n"
                                                      "        cv_varnames:\n"
                                                      "            - \"fragment_position.x\"\n"
                                                      "            - \"fragment_position.y\"\n"
                                                      "            - \"fragment_position.z\"\n"
                                                      "        wall_bias_varnames:\n"
                                                      "            - \"wall_lower.bias\"\n"
                                                      "            - \"wall_upper.bias\"\n"
                                                      "        metad_bias_varname: \"metad_position.bias\"\n"
                                                      "        cv_filename_hint: \"CV_fragment_meac\"\n"
                                                      "        hills_filename_hint: \"HILLS_fragment_meac\"\n"
                                                      "        \n";
}


BOOST_TEST_DONT_PRINT_LOG_VALUE(f::StepFunction);


BOOST_AUTO_TEST_SUITE(read_yaml_runfile_ts)

    BOOST_AUTO_TEST_CASE(complete_correct_example_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_cv = td.createTemporaryFileInDir("CV_fragment_meac",
                                                                               "content does not matter");
        bf::path temp_cv_path = temp_cv->getFilePath();

        std::shared_ptr<TempTextFile>  temp_hills = td.createTemporaryFileInDir("HILLS_fragment_meac",
                                                                                  "content does not matter");
        bf::path temp_hills_path = temp_hills->getFilePath();

        std::string temp_runfile_content = fmt::format(yaml_correct_runfile_template, td_path.generic_string());

        std::shared_ptr<TempTextFile>  temp_runfile = td.createTemporaryFileInDir("runfile.yaml",
                                                                                  temp_runfile_content);
        bf::path temp_runfile_path = temp_runfile->getFilePath();

        f::GeneralOptions options;

        // TEST

        f::read_yaml_runfile(options, temp_runfile_path.generic_string());

        options.search_directory.count("all");

        BOOST_TEST(options.run_title == "Reweight & fragment reconstruction");

        BOOST_TEST( options.search_directory.count("all") == 1);
        BOOST_REQUIRE( options.search_directory.count("all") == 1);
        BOOST_TEST( options.search_directory.at("all") == td_path.generic_string());

        BOOST_TEST( f::g_all_steps_list.size() == 1);
        BOOST_REQUIRE( f::g_all_steps_list.size() == 1);
        BOOST_TEST( f::g_all_steps_list.at(0).function == f::StepFunction::fes_data);


    }

    BOOST_AUTO_TEST_CASE(missing_general_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_cv = td.createTemporaryFileInDir("CV_fragment_meac",
                                                                             "content does not matter");
        bf::path temp_cv_path = temp_cv->getFilePath();

        std::shared_ptr<TempTextFile>  temp_hills = td.createTemporaryFileInDir("HILLS_fragment_meac",
                                                                                "content does not matter");
        bf::path temp_hills_path = temp_hills->getFilePath();

        std::string temp_runfile_content = fmt::format(yaml_runfile_template_missing_general, td_path.generic_string());

        std::shared_ptr<TempTextFile>  temp_runfile = td.createTemporaryFileInDir("runfile.yaml",
                                                                                  temp_runfile_content);
        bf::path temp_runfile_path = temp_runfile->getFilePath();

        f::GeneralOptions options;

        // TEST

        BOOST_CHECK_THROW(f::read_yaml_runfile(options, temp_runfile_path.generic_string()), std::runtime_error);

    }

    BOOST_AUTO_TEST_CASE(missing_searchdir_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_cv = td.createTemporaryFileInDir("CV_fragment_meac",
                                                                             "content does not matter");
        bf::path temp_cv_path = temp_cv->getFilePath();

        std::shared_ptr<TempTextFile>  temp_hills = td.createTemporaryFileInDir("HILLS_fragment_meac",
                                                                                "content does not matter");
        bf::path temp_hills_path = temp_hills->getFilePath();


        std::shared_ptr<TempTextFile>  temp_runfile = td.createTemporaryFileInDir("runfile.yaml",
                                                                                  yaml_runfile_template_missing_search_dir);
        bf::path temp_runfile_path = temp_runfile->getFilePath();

        f::GeneralOptions options;

        // TEST

        BOOST_CHECK_THROW(f::read_yaml_runfile(options, temp_runfile_path.generic_string()), std::runtime_error);

    }

    BOOST_AUTO_TEST_CASE(missing_fes_data_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_cv = td.createTemporaryFileInDir("CV_fragment_meac",
                                                                             "content does not matter");
        bf::path temp_cv_path = temp_cv->getFilePath();

        std::shared_ptr<TempTextFile>  temp_hills = td.createTemporaryFileInDir("HILLS_fragment_meac",
                                                                                "content does not matter");
        bf::path temp_hills_path = temp_hills->getFilePath();

        std::string temp_runfile_content = fmt::format(yaml_correct_runfile_template_missing_fes_data, td_path.generic_string());

        std::shared_ptr<TempTextFile>  temp_runfile = td.createTemporaryFileInDir("runfile.yaml",
                                                                                  temp_runfile_content);
        bf::path temp_runfile_path = temp_runfile->getFilePath();

        f::GeneralOptions options;

        // TEST

        BOOST_CHECK_THROW(f::read_yaml_runfile(options, temp_runfile_path.generic_string()), std::runtime_error);

    }

    BOOST_AUTO_TEST_CASE(missing_steps_tc, *boost::unit_test::tolerance(0.001)) {
        // PRELUDE
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_cv = td.createTemporaryFileInDir("CV_fragment_meac",
                                                                             "content does not matter");
        bf::path temp_cv_path = temp_cv->getFilePath();

        std::shared_ptr<TempTextFile>  temp_hills = td.createTemporaryFileInDir("HILLS_fragment_meac",
                                                                                "content does not matter");
        bf::path temp_hills_path = temp_hills->getFilePath();

        std::string temp_runfile_content = fmt::format(yaml_correct_runfile_template_missing_steps, td_path.generic_string());

        std::shared_ptr<TempTextFile>  temp_runfile = td.createTemporaryFileInDir("runfile.yaml",
                                                                                  temp_runfile_content);
        bf::path temp_runfile_path = temp_runfile->getFilePath();

        f::GeneralOptions options;

        // TEST

        BOOST_CHECK_THROW(f::read_yaml_runfile(options, temp_runfile_path.generic_string()), std::runtime_error);

    }



BOOST_AUTO_TEST_SUITE_END();