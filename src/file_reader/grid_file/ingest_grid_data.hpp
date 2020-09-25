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

#ifndef FESUTILS_INGEST_GRID_DATA_HPP
#define FESUTILS_INGEST_GRID_DATA_HPP

#include "../../GeneralOptions.h"
#include "GridData.hpp"

#include <boost/filesystem.hpp>
namespace bf = boost::filesystem;

namespace fesutils {
    std::shared_ptr<GridData> ingest_grid_data(const GeneralOptions& options,
                                              const bf::path& grid_path,
                                              const PlumedDatHeader& header_grid,
                                              const size_t& value_index,
                                              const std::vector<size_t>& variable_index);

    bool ingest_grid_data_from_bindump(const GeneralOptions& options,
                                     const bf::path& grid_file_path,
                                     std::shared_ptr<GridData> griddata_container,
                                     const PlumedDatHeader& header_grid,
                                     unsigned int num_axis,
                                     const size_t& value_index,
                                     const std::vector<size_t>& variable_index,
                                     const bf::path& bindump_path);

    bool ingest_grid_data_from_originalfile(const GeneralOptions& options,
                                            const bf::path& grid_file_path,
                                            std::shared_ptr<GridData> griddata_container,
                                            const PlumedDatHeader& header_grid,
                                            unsigned int num_axis,
                                            const size_t& value_index,
                                            const std::vector<size_t>& variable_index);

}

#endif //FESUTILS_INGEST_GRID_DATA_HPP
