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

#include "help.hpp"

#include <boost/log/trivial.hpp>


namespace fesutils {

    // LCOV_EXCL_START
    // Reason for coverage exclusion: purely console printing function

    int do_help(const std::string& command) {
        /*
        if(command == "final_bias_reweight")  {

        }else {

        }
         */

        BOOST_LOG_TRIVIAL(info) << "See documentation for detailed help.";
        BOOST_LOG_TRIVIAL(info) << "Available subcommands: ";
        BOOST_LOG_TRIVIAL(info) << "final_bias_reweight - Reweight frames from a MetaD trajectory using a final bias grid.";

        return 0;
    }

    // LCOV_EXCL_STOP
}