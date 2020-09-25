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


#include "GeneralOptions.h"

#include <boost/algorithm/string.hpp>

#include <boost/log/trivial.hpp>


namespace fesutils {

    LogLevel string_to_LogLevel(const std::string& loglevel_as_str) {
        const std::string loglevel_str_lower = boost::algorithm::to_lower_copy(loglevel_as_str);

        if(loglevel_str_lower == "debug") {
            return LogLevel::debug;
        }else if (loglevel_str_lower == "info") {
            return LogLevel::info;
        }else if (loglevel_str_lower == "warning") {
            return LogLevel::warning;
        }else if (loglevel_str_lower == "error") {
            return LogLevel::error;
        }else {
            BOOST_LOG_TRIVIAL(warning) << "Unknown LogLevel \""<< loglevel_as_str << "\", selected debug level instead.";
            return LogLevel::debug;
        }
    }

    std::string logLevel_to_string(const LogLevel& loglevel) {
        if(loglevel == LogLevel::debug) {
            return "debug";
        } else if(loglevel == LogLevel::info) {
            return "info";
        } else if(loglevel == LogLevel::warning) {
            return "warning";
        } else if(loglevel == LogLevel::error) {
            return "error";
        } else {
            // LCOV_EXCL_START
            // Reason for coverage exclusion: enum class incorrect value are difficult to cleanly produce for unit testing
            return "<unknown loglevel enum>";
            // LCOV_EXCL_STOP
        }
    }

    BinaryCachePolicy string_to_BinaryCachePolicy(const std::string& binaryCachePolicy_as_str) {
        const std::string bcp_str_lower = boost::algorithm::to_lower_copy(binaryCachePolicy_as_str);

        if(bcp_str_lower == "always") {
            return BinaryCachePolicy::always;
        } else if(bcp_str_lower == "never") {
            return BinaryCachePolicy::never;
        }else {
            BOOST_LOG_TRIVIAL(warning) << "Unknown binary cache policy \""<< binaryCachePolicy_as_str <<
                            "\", will never cache instead for this run.";
            return BinaryCachePolicy::never;
        }
    }

    std::string binaryCachePolicy_to_string(const BinaryCachePolicy& binaryCachePolicy) {
        if(binaryCachePolicy == BinaryCachePolicy::never) {
            return "never";
        } else if(binaryCachePolicy == BinaryCachePolicy::always) {
            return "always";
        } else {
            // LCOV_EXCL_START
            // Reason for coverage exclusion: enum class incorrect value are difficult to cleanly produce for unit testing
            return "<unknown binary cache enum>";
            // LCOV_EXCL_STOP
        }
    }
}