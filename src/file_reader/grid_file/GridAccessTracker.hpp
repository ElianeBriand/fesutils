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


#ifndef FESUTILS_GRIDACCESSTRACKER_HPP
#define FESUTILS_GRIDACCESSTRACKER_HPP

#include <mutex>
#include <vector>

#include "GridData.hpp"

namespace fesutils {

    class GridData;

    class GridAccessTracker {
    public:


        bool increment_at_coord_rangechecked(const std::vector<double>& coord,
                                             std::vector<long int>& idx_buffer);

        bool all_voxel_accessed_n_times_or_less(const int n);

    protected:
        friend GridData;
        explicit GridAccessTracker(GridData* grid_);


        long int indices_to_globalindex(const std::vector<long int>& indices);

        GridData* grid;
        unsigned int num_axis;
        unsigned int num_voxels;
        std::vector<unsigned int> bins_per_axis;

        std::mutex write_access_tracker_grid_mutex;
        std::vector<int> write_access_tracker_grid;

    };

}




#endif //FESUTILS_GRIDACCESSTRACKER_HPP
