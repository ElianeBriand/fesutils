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


#ifndef FESUTILS_CV_SPACE_COVERAGE_HPP
#define FESUTILS_CV_SPACE_COVERAGE_HPP

#include "../file_reader/grid_file/GridAccessTracker.hpp"
#include "../file_reader/cv_file/CVData.hpp"

namespace fesutils {

        /** For a given grid, track which voxel are sampled by a given CV data run
         *
         * This is used by block analysis and coverage analysis, to know which voxels are reached in a block
         *
         * \param cv_data
         * \param gridData
         * \return
         */
        std::shared_ptr<GridAccessTracker> track_sampled_voxel_in_CVData(const CVData& cv_data,
                                                                         const GridData& gridData);

}

#endif //FESUTILS_CV_SPACE_COVERAGE_HPP
