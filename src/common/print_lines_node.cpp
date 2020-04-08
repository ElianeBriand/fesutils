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
#include "print_lines_node.hpp"

#include <boost/log/trivial.hpp>

// LCOV_EXCL_START
// Reason for coverage exclusion: not unit testing pure io

fesutils::print_lines_node::print_lines_node(const std::string& prefix) {

}



bool fesutils::print_lines_node::operator()(const std::vector<std::string>& lines) {
    if (lines.empty())
        return false;
    for(const auto& line: lines) {
        BOOST_LOG_TRIVIAL(info) << this->prefix << line;
    }
    return true;
}


// LCOV_EXCL_STOP
