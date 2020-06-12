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

#ifndef FESUTILS_FRONTEND_COMMON_H
#define FESUTILS_FRONTEND_COMMON_H

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>

inline void setupLogPrinting(bool noDebug = false, bool onlyError = false, std::string prefix = "")
{
    if(noDebug)
    {
        if(onlyError)
        {
            boost::log::core::get()->set_filter
                    (boost::log::trivial::severity >= boost::log::trivial::error);
        }else {
            boost::log::core::get()->set_filter
                    (boost::log::trivial::severity >= boost::log::trivial::info);
        }

    }else {
        boost::log::core::get()->set_filter
                (boost::log::trivial::severity >= boost::log::trivial::debug);
    }


    auto console_logger = boost::log::add_console_log(std::cout);
    console_logger->set_formatter([prefix](boost::log::record_view const &rec, boost::log::formatting_ostream &strm) {
        if (rec[boost::log::trivial::severity] == boost::log::trivial::trace) {
            strm << prefix << " T  "; //         use TRACE_LOG(); macro for auto file:line:function
        } else if (rec[boost::log::trivial::severity] == boost::log::trivial::debug) {
            strm << prefix << "{D} ";
        } else if (rec[boost::log::trivial::severity] == boost::log::trivial::info) {
            strm << prefix << "    ";
        } else if (rec[boost::log::trivial::severity] == boost::log::trivial::warning) {
            strm << prefix << "[!] ";
        } else if (rec[boost::log::trivial::severity] >= boost::log::trivial::error) {
            strm << prefix << "[E] ";
        }

        strm << rec[boost::log::expressions::smessage];
    });
}

inline void logToFile(const std::string& filepath) {
    auto file_logger = boost::log::add_file_log
            (
                    boost::log::keywords::file_name = filepath ,
                    boost::log::keywords::open_mode = std::ios_base::app,
                    boost::log::keywords::auto_flush = true
            );

    file_logger->set_formatter([](boost::log::record_view const &rec, boost::log::formatting_ostream &strm) {
        if (rec[boost::log::trivial::severity] == boost::log::trivial::trace) {
            strm << " T  "; //         use TRACE_LOG(); macro for auto file:line:function
        } else if (rec[boost::log::trivial::severity] == boost::log::trivial::debug) {
            strm << "{D} ";
        } else if (rec[boost::log::trivial::severity] == boost::log::trivial::info) {
            strm << "    ";
        } else if (rec[boost::log::trivial::severity] == boost::log::trivial::warning) {
            strm << "[!] ";
        } else if (rec[boost::log::trivial::severity] >= boost::log::trivial::error) {
            strm << "[E] ";
        }

        strm << rec[boost::log::expressions::smessage];
    });
}




#endif //FESUTILS_FRONTEND_COMMON_H
