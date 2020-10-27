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

#ifndef FESUTILS_STEP_FES_DATA_HPP
#define FESUTILS_STEP_FES_DATA_HPP

#include "../GeneralOptions.h"

namespace fesutils {
    /**
     * Run the fes_data step for a given parameter name. (parameters are fetched from the global store which is filled during
     * parsing of the runfile)
     *
     * \param options options for the run
     * \param parameter_set_name label of the parameter set in the runfile
     * \return true if the step completed without error
     */
    bool run_fes_data_step(GeneralOptions options, const std::string& parameter_set_name);
}

#endif //FESUTILS_STEP_FES_DATA_HPP
