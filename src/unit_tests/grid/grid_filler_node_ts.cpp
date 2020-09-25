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

#include <iostream>

#include <vector>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>

#include <boost/test/unit_test.hpp>

#include "../../grid/grid_utils.hpp"
#include "../MiscFixture.hpp"
#include "../../file_reader/grid_file/grid_filler_node.hpp"

namespace f = fesutils;
namespace tt = boost::test_tools;


BOOST_AUTO_TEST_SUITE(grid_filler_node_ts)


BOOST_AUTO_TEST_SUITE_END();