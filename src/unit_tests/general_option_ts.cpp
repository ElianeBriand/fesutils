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

#include "../GeneralOptions.h"
namespace f = fesutils;

BOOST_TEST_DONT_PRINT_LOG_VALUE(f::LogLevel);
BOOST_TEST_DONT_PRINT_LOG_VALUE(f::BinaryCachePolicy);

BOOST_AUTO_TEST_SUITE(general_option_ts)

    BOOST_AUTO_TEST_CASE(loglevel_str_conversion_tc, *boost::unit_test::tolerance(0.001)) {

        std::vector<std::tuple<std::string,f::LogLevel>> values = { {"debug", f::LogLevel::debug},
                                                                    {"info", f::LogLevel::info},
                                                                    {"warning", f::LogLevel::warning},
                                                                    {"error", f::LogLevel::error}};

        for(const auto& tuple: values) {
            f::LogLevel ll =  f::string_to_LogLevel(std::get<0>(tuple));
            BOOST_TEST_INFO(std::get<0>(tuple));
            BOOST_TEST(ll == std::get<1>(tuple));
            std::string ll_str = logLevel_to_string(ll);
            BOOST_TEST_INFO(std::get<0>(tuple));
            BOOST_TEST(ll_str == std::get<0>(tuple));
        }

        // test for unknown log level
        f::LogLevel ll_default =  f::string_to_LogLevel("nonexistent");
        BOOST_TEST(ll_default == f::LogLevel::debug);

    }

    BOOST_AUTO_TEST_CASE(binarycachepolicy_str_conversion_tc, *boost::unit_test::tolerance(0.001)) {

        std::vector<std::tuple<std::string,f::BinaryCachePolicy>> values = { {"always", f::BinaryCachePolicy::always},
                                                                    {"never", f::BinaryCachePolicy::never}};

        for(const auto& tuple: values) {
            f::BinaryCachePolicy bcp =  f::string_to_BinaryCachePolicy(std::get<0>(tuple));
            BOOST_TEST_INFO(std::get<0>(tuple));
            BOOST_TEST(bcp == std::get<1>(tuple));
            std::string bcp_str = binaryCachePolicy_to_string(bcp);
            BOOST_TEST_INFO(std::get<0>(tuple));
            BOOST_TEST(bcp_str == std::get<0>(tuple));
        }

        // test for unknown log level
        f::BinaryCachePolicy bcp_default =  f::string_to_BinaryCachePolicy("nonexistent");
        BOOST_TEST(bcp_default == f::BinaryCachePolicy::never);

    }


BOOST_AUTO_TEST_SUITE_END();