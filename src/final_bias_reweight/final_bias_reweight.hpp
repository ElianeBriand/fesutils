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

#ifndef FESUTILS_FINAL_BIAS_REWEIGHT_HPP
#define FESUTILS_FINAL_BIAS_REWEIGHT_HPP

#include <string>
#include <optional>

#include <boost/filesystem.hpp>
namespace bf = boost::filesystem;


#include "../GeneralOptions.h"


namespace fesutils {

    struct final_bias_reweight_args {
        bf::path cvfile_path;
        bf::path metadgrid_path;
        std::string ranges_min_rawstr;
        std::string ranges_max_rawstr;

        std::optional<std::string> biasfield;
    };

    /**
     * Entry point for the final_bias_reweight subcommand.
     *
     * \param options General options passed on the command line
     * \param args Specific arguments for the command, passed as a struct
     */
    int do_final_bias_reweight(GeneralOptions& options,
                                const final_bias_reweight_args& args);

}


#endif //FESUTILS_FINAL_BIAS_REWEIGHT_HPP
