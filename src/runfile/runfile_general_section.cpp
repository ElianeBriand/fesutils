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


#include "runfile_general_section.hpp"
#include "../frontend_common.h"

#include <boost/log/trivial.hpp>

namespace fesutils {

    void parse_general_section(GeneralOptions& options, YAML::Node& general_node) {


        if (general_node["title"]) {
            options.run_title = general_node["title"].as<std::string>();
        } else {
            options.run_title = "Untitled run";
        }

        if (general_node["log_level"]) {
            options.loglevel = string_to_LogLevel(general_node["log_level"].as<std::string>());
        } else {
            options.loglevel = LogLevel::info;
        }
        // LCOV_EXCL_START
        // Reason for coverage exclusion: difficult to check
        if (options.loglevel == fesutils::LogLevel::debug) {
            setup_log_level_filter(false, false);
            BOOST_LOG_TRIVIAL(debug) << "Debug messages enabled";
        } else if (options.loglevel == fesutils::LogLevel::error) {
            setup_log_level_filter(false, true);
        } else {
            setup_log_level_filter(true, false);
        }
        // LCOV_EXCL_STOP

        if (general_node["binary_cache_policy"]) {
            options.binaryCachePolicy = string_to_BinaryCachePolicy(general_node["binary_cache_policy"].as<std::string>());
        } else {
            options.binaryCachePolicy = BinaryCachePolicy::never;
        }

        if (general_node["ui_bling"]) {
            options.ui_bling = general_node["ui_bling"].as<bool>();

        } else {
            options.ui_bling = false;
        }


    }

}

