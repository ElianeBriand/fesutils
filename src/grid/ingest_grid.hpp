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

#ifndef FESUTILS_INGEST_GRID_HPP
#define FESUTILS_INGEST_GRID_HPP

#include <memory>

#include "Grid.hpp"
#include "../GeneralOptions.h"

namespace fesutils {

    /** Parse a FES grid file into a Grid object, given general options, and fes-specific options
     *
     * \param options General options (I/O, notably)
     * \param grid_path path to the FES grid fgile
     * \param header_grid parsed header for the file
     * \param override_freenergy_field_name optionally, a specific field name for the free energy
     * \return
     */
    std::shared_ptr<Grid> ingest_fes_grid(const GeneralOptions& options,
                                          const std::string& grid_path,
                                          PlumedDatHeader& header_grid,
                                          const std::optional<std::string>& override_freenergy_field_name  = std::optional<std::string>());

}

#endif //FESUTILS_INGEST_GRID_HPP
