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

#include "../GeneralOptions.h"


namespace fesutils {

    /**
     * Entry point for the final_bias_reweight subcommand.
     *
     * \param options General options passed on the command line
     * \param cvfile_path Path to the file containing the collective variable (cv)
     * \param metadgrid_path Path to the file containing the metadynamic grid
     * \param ranges_min_rawstr Minimal value for the CV (empty string if not passed)
     * \param ranges_max_rawstr Maximal value for the CV (empty string if not passed)
     */
    void do_final_bias_reweight(GeneralOptions& options,
                                const std::string& cvfile_path,
                                const std::string& metadgrid_path,
                                const std::string& ranges_min_rawstr,
                                const std::string& ranges_max_rawstr);

}


#endif //FESUTILS_FINAL_BIAS_REWEIGHT_HPP
