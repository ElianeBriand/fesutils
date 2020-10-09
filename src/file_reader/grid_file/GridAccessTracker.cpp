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

#include "GridAccessTracker.hpp"

#include <fmt/format.h>

namespace fesutils {


    GridAccessTracker::GridAccessTracker(GridData* grid_) :
    grid(grid_),
    num_axis(grid->get_num_axis()),
    num_voxels(grid->get_num_voxels()),
    bins_per_axis(grid->get_bins_per_axis()),
    write_access_tracker_grid(num_voxels, 0)
    {
        BOOST_LOG_TRIVIAL(info) << fmt::format(
                "GridAccessTracker: Allocating a grid of size {:} bytes",(this->num_voxels * sizeof(int)));
    }

    bool GridAccessTracker::increment_at_coord_rangechecked(const std::vector<double>& coord,
                                                            std::vector<long>& idx_buffer) {
        bool in_range =  grid->coord_to_indices_rangechecked(coord,
                                                       idx_buffer);
        if(!in_range) {
            // LCOV_EXCL_START
            // Reason for coverage exclusion: defensive check, as GridData already checks for range
            return false;
            // LCOV_EXCL_STOP
        }

        const long int global_idx = this->indices_to_globalindex(idx_buffer);

        {
            std::scoped_lock lock(this->write_access_tracker_grid_mutex);
            this->write_access_tracker_grid[global_idx] += 1;
        }

        return true;
    }

    long int GridAccessTracker::indices_to_globalindex(const std::vector<long>& indices) {
        long int global_index = 0;
        for(unsigned int axis = 0; axis < this->num_axis; axis++) {
            long int coefficient = 1;
            if(axis > 0) {
                for(unsigned int i = 0; i <= (axis - 1); i++) {
                    coefficient *= this->bins_per_axis[i];
                }
            }
            global_index += coefficient * indices[axis];
        }
        return global_index;
    }

    bool GridAccessTracker::all_voxel_accessed_n_times_or_less(const int n) {
        std::scoped_lock lock(this->write_access_tracker_grid_mutex);
        return std::all_of(this->write_access_tracker_grid.cbegin(),
                           this->write_access_tracker_grid.cend(),
                           [&](const int& num_write_access){ return num_write_access <= n; });
    }
}