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

#ifndef FESUTILS_GENERALOPTIONS_H
#define FESUTILS_GENERALOPTIONS_H

#include <cstdlib>
#include <map>
namespace fesutils {


    struct FileIOPerfOptions {
        size_t binaryFileReadBufferSize = 1024 * 1024;

    };

    enum class LogLevel {
        debug,
        info,
        warning,
        error
    };



    LogLevel string_to_LogLevel(const std::string& loglevel_as_str);
    std::string logLevel_to_string(const LogLevel& loglevel);


    enum class BinaryCachePolicy {
        always,
        //checktimestamp,
        never
    };

    BinaryCachePolicy string_to_BinaryCachePolicy(const std::string& binaryCachePolicy_as_str);
    std::string binaryCachePolicy_to_string(const BinaryCachePolicy& binaryCachePolicy);



    struct GeneralOptions {

        std::string run_title = "Untitled";
        LogLevel loglevel = LogLevel::info;
        BinaryCachePolicy binaryCachePolicy = BinaryCachePolicy::never;
        bool ui_bling = false;

        std::map<std::string, std::string> search_directory;

        // Performance tuning

        FileIOPerfOptions fileIOPerfOptions;

    };
}




#endif //FESUTILS_GENERALOPTIONS_H
