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

#include "frontend_common.h"

// LCOV_EXCL_START
// Reason for coverage exclusion: Logging/IO are very difficult to cover with unit test (not setup for mocking)


namespace {
    unsigned int indentation_number = 0;
    std::string formatting_prefix;
    boost::shared_ptr<boost::log::sinks::synchronous_sink<boost::log::sinks::basic_text_ostream_backend<char>>> console_logger;
}


void setup_log_formating_internal();

void setup_log_formating(std::string prefix) {
    formatting_prefix = prefix;
    setup_log_formating_internal();
}

void setup_log_formating_internal() {
    std::string indentation;
    for(unsigned int i = indentation_number; i > 0; i--) {
        indentation += " ";
    }

    if(console_logger == nullptr) {
        console_logger = boost::log::add_console_log(std::cout);
    }

    console_logger->set_formatter([=](boost::log::record_view const &rec, boost::log::formatting_ostream &strm) {
        if (rec[boost::log::trivial::severity] == boost::log::trivial::trace) {
            strm << formatting_prefix << " T  " << indentation; //         use TRACE_LOG(); macro for auto file:line:function
        } else if (rec[boost::log::trivial::severity] == boost::log::trivial::debug) {
            strm << formatting_prefix << "{D} " << indentation;
        } else if (rec[boost::log::trivial::severity] == boost::log::trivial::info) {
            strm << formatting_prefix << "    " << indentation;
        } else if (rec[boost::log::trivial::severity] == boost::log::trivial::warning) {
            strm << formatting_prefix << "[!] " << indentation;
        } else if (rec[boost::log::trivial::severity] >= boost::log::trivial::error) {
            strm << formatting_prefix << "[E] " << indentation;
        }

        strm << rec[boost::log::expressions::smessage];
    });
}



void setup_log_level_filter(bool noDebug, bool onlyError)
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

}

void logToFile(const std::string& filepath) {
    auto file_logger = boost::log::add_file_log
            (
                    boost::log::keywords::file_name = filepath ,
                    boost::log::keywords::open_mode = std::ios_base::app,
                    boost::log::keywords::auto_flush = true
            );

    std::string indentation;
    for(unsigned int i = indentation_number; i > 0; i--) {
        indentation += " ";
    }

    file_logger->set_formatter([=](boost::log::record_view const &rec, boost::log::formatting_ostream &strm) {
        if (rec[boost::log::trivial::severity] == boost::log::trivial::trace) {
            strm << formatting_prefix << " T  " << indentation; //         use TRACE_LOG(); macro for auto file:line:function
        } else if (rec[boost::log::trivial::severity] == boost::log::trivial::debug) {
            strm << formatting_prefix << "{D} " << indentation;
        } else if (rec[boost::log::trivial::severity] == boost::log::trivial::info) {
            strm << formatting_prefix << "    " << indentation;
        } else if (rec[boost::log::trivial::severity] == boost::log::trivial::warning) {
            strm << formatting_prefix << "[!] " << indentation;
        } else if (rec[boost::log::trivial::severity] >= boost::log::trivial::error) {
            strm << formatting_prefix << "[E] " << indentation;
        }

        strm << rec[boost::log::expressions::smessage];
    });
}

// LCOV_EXCL_STOP

void setup_log_formating_increase_indentation() {
    indentation_number += 3;
    setup_log_formating_internal();
}


void setup_log_formating_decrease_indentation() {
    if(indentation_number >= 3) {
        indentation_number -= 3;
        setup_log_formating_internal();
    } else {
        // LCOV_EXCL_START
        // Reason for coverage exclusion: Cannot be unit tested reasonably
        BOOST_LOG_TRIVIAL(debug) << "[FIXME] setup_log_formating_decrease_indentation called while indentation was already 0.";
        // LCOV_EXCL_STOP
    }
}

std::string non_boost_log_get_space_prefix() {
    std::string indentation = "    ";
    for(unsigned int i = indentation_number; i > 0; i--) {
        indentation += " ";
    }
    return indentation;
}

