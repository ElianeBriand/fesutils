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

#include "reconstruction_verynaive.hpp"
#include "reconstruction_utils.hpp"
#include "../grid/grid_utils.hpp"

#include <boost/log/trivial.hpp>


namespace fesutils {

    std::shared_ptr<Grid> reconstruction_verynaive_algorithm(const std::vector<std::shared_ptr<Grid>>& fes_grids,
                                                             const std::vector<PlumedDatHeader>& fes_grid_headers,
                                                             const FragmentInfo& fragmentInfo) {


        BOOST_LOG_TRIVIAL(info) << "Creating result grid";

        // Make header with the minimal dimensions
        PlumedDatHeader result_header = grid_from_intersection_of_grids(fragmentInfo, fes_grid_headers);

        std::shared_ptr<Grid> result_grid = std::make_shared<Grid>(result_header);




        BOOST_LOG_TRIVIAL(info) << "Populating grid";
        std::vector<double> coord_vector((size_t) result_grid->num_dims);
        std::vector<size_t> idx_in_bin_centers((size_t) result_grid->num_dims, 0);
        while(true) {

            // Populate grid
            for(int curr_dim = 0; curr_dim < result_grid->num_dims; curr_dim++) {
                coord_vector[curr_dim] = result_grid->bin_center.at(curr_dim).at(idx_in_bin_centers.at(curr_dim));
            }
            if(!result_grid->check_inrange(coord_vector)) {
                BOOST_LOG_TRIVIAL(error) << "result_grid->check_inrange failed for generated coord_vector = " << coord_vector;
                throw std::runtime_error("Coordinate vector not in range");
            }

            double fes_value = 0.0;
            for(const auto & fes_grid : fes_grids) {
                if(!fes_grid->check_inrange(coord_vector)) {
                    BOOST_LOG_TRIVIAL(error) << "fes_grid->check_inrange failed for generated coord_vector = " << coord_vector;
                    throw std::runtime_error("Coordinate vector not in range");
                }
                fes_value += fes_grid->get_inrange(coord_vector);
            }
            result_grid->set_inrange(coord_vector, fes_value);

            // Increment indexes until reaching the end
            if(!increment_grid_indexes(*result_grid, idx_in_bin_centers )) {
                break;
            }
        }
        BOOST_LOG_TRIVIAL(info) << "Done populating grid";


        return result_grid;
    }

    void iterate_on_coord_and_fill_grid(int remaining_dims,
                        std::vector<double>& coord_buffer,
                        std::shared_ptr<Grid>& result_grid,
                        const std::vector<std::shared_ptr<Grid>>& fes_grids){
        if(remaining_dims == 0) {
            // coord_buffer is ready
            return;
        } else {
            size_t curr_dim_index = result_grid->num_dims - remaining_dims;
            const std::vector<double>& coord_values = result_grid->bin_center[curr_dim_index];
            for(double coord_value : coord_values) {
                coord_buffer[curr_dim_index] = coord_value;
                iterate_on_coord_and_fill_grid(remaining_dims - 1,
                        coord_buffer,result_grid, fes_grids);
            }
        }
    }


}