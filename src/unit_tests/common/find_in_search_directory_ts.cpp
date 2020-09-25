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

#include "../../common/find_in_search_directory.hpp"
#include "../TempDirectory.hpp"

namespace f = fesutils;

BOOST_AUTO_TEST_SUITE(find_in_search_directory_ts)

    BOOST_AUTO_TEST_CASE(find_in_all_tc, *boost::unit_test::tolerance(0.001)) {
        TempDirectory td;
        bf::path td_path = td.getDirPath();
        std::shared_ptr<TempTextFile>  temp_file = td.createTemporaryFileInDir("fragment_dummy", "content");
        f::GeneralOptions options;

        options.search_directory["all"] = td.getDirPathAsStr();

        bf::path p_1 = f::find_cv_path_from_hint(options, "fragment_dummy");
        BOOST_TEST(p_1 == temp_file->getFilePath());
        BOOST_CHECK_THROW(f::find_cv_path_from_hint(options, "fragment_nonexistent"), std::runtime_error);

        bf::path p_2 = f::find_hills_path_from_hint(options, "fragment_dummy");
        BOOST_TEST(p_2 == temp_file->getFilePath());
        BOOST_CHECK_THROW(f::find_hills_path_from_hint(options, "fragment_nonexistent"), std::runtime_error);

        bf::path p_3 = f::find_grid_path_from_hint(options, "fragment_dummy");
        BOOST_TEST(p_3 == temp_file->getFilePath());
        BOOST_CHECK_THROW(f::find_grid_path_from_hint(options, "fragment_nonexistent"), std::runtime_error);

        bf::path p_4 = f::find_any_path_from_hint(options, "fragment_dummy");
        BOOST_TEST(p_4 == temp_file->getFilePath());
        BOOST_CHECK_THROW(f::find_any_path_from_hint(options, "fragment_nonexistent"), std::runtime_error);


    }


BOOST_AUTO_TEST_SUITE_END();