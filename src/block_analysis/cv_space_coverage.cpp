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


#include "cv_space_coverage.hpp"

namespace fesutils {


    std::shared_ptr<GridAccessTracker> count_sampled_voxel_in_CVData(const CVData& cv_data,
                                                                     const GridData& gridData) {
        std::shared_ptr<GridAccessTracker> gat = std::make_shared<GridAccessTracker>(gridData);

        const size_t num_records = cv_data.get_num_record();
        const unsigned int num_cv_dimensions = cv_data.get_num_cv_dimensions();

        std::vector<double> datapoint_buffer(cv_data.get_total_dim_in_one_record(), 0.0);
        std::vector<double> cv_buffer(num_cv_dimensions, 0.0);
        std::vector<long int> idx_buffer(num_cv_dimensions, 0);

        for(size_t i = 0; i < num_records; i++) {
            cv_data.getDatapoint(i, datapoint_buffer);
            std::copy(datapoint_buffer.begin(), datapoint_buffer.begin() + num_cv_dimensions, cv_buffer.begin());
            gat->increment_at_coord_rangechecked(cv_buffer, idx_buffer);
        }

        return gat;
    }
}