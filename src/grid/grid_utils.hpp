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

#ifndef FESUTILS_GRID_UTILS_HPP
#define FESUTILS_GRID_UTILS_HPP

#include <vector>

#include "../file_reader/PlumedDatHeader.hpp"



namespace fesutils {





    /** Generate the grid tensor dimensions from the header.
     *
     * PRECONDITION: check_fields_have_required_attribute_for_grid has returned true
     *
     * \param header
     * \return
     * \sa check_fields_have_required_attribute_for_grid
     */
    std::vector<long int> grid_dims_from_header(const PlumedDatHeader& header, const std::vector<size_t>& which_fields);

    /**
     * Compute the size of the bins, for each dimension, given a header.
     *
     * PRECONDITION: check_fields_have_required_attribute_for_grid has returned true
     *
     * \param header
     * \return
     */
    std::vector<double> grid_bin_sizes_from_header(const PlumedDatHeader& header, const std::vector<size_t>& which_fields);

    std::vector<double> extract_grid_min_val(const PlumedDatHeader& header, const std::vector<size_t>& which_fields);
    std::vector<double> extract_grid_max_val(const PlumedDatHeader& header, const std::vector<size_t>& which_fields);

    std::vector<std::vector<double>> compute_grid_bin_edges(const std::vector<double>& min_vals,
                                                            const std::vector<double>& max_vals,
                                                            const std::vector<double>& bin_sizes,
                                                            const std::vector<long int>& dims);

    std::vector<std::vector<double>> compute_grid_bin_center(const std::vector<std::vector<double>>& bin_edges);

    // std::vector<taco::ModeFormatPack> all_dense_from_dims(const std::vector<int>& dims);

    /** Increment indexes for axis value to traverse all grid points. Returns false when finished
     *
     * \param grid grid to traverse by indexes
     * \param indexes a pre-allocated vector with N dims, same as grids
     * \return true if increment successful, false if at the end of the traversal
     */
     /*
    template <typename T>
    inline bool increment_grid_indexes(const Grid& grid, std::vector<T>& indexes) {
        // Increment index
        for(int curr_dim = 0; curr_dim < grid.num_dims; curr_dim++) {
            // If current coordinate index lower than max index, increment this coordinate then go on to outer loop
            if(indexes[curr_dim] < (grid.dims[curr_dim] - 1)) {
                indexes[curr_dim]++;
                break;
            }else {
                // Else : check if this is the last coordinate
                if(curr_dim == (grid.num_dims - 1)) {
                    // If it is, we are trying to increment last index past the end: we have finished enumerating indexes
                    // end outer loop
                    return false;
                } else {
                    // If it isn't, carry increment to next coordinate
                    indexes[curr_dim] = 0;
                }
            }
        }
        return true;
    }
*/
    /** Increment indexes for axis value to traverse all grid points. Returns false when finished
 *
 * \param grid grid to traverse by indexes
 * \param indexes a pre-allocated vector with N dims, same as grids
 * \return true if increment successful, false if at the end of the traversal
 */
 /*
    template <typename T>
    inline bool increment_grid_indexes_last_first(const Grid& grid, std::vector<T>& indexes) {
        // Increment index
        for(int curr_dim = (grid.num_dims - 1); curr_dim >= 0; curr_dim--) {
            // If current coordinate index lower than max index, increment this coordinate then go on to outer loop
            if(indexes[curr_dim] < (grid.dims[curr_dim] - 1)) {
                indexes[curr_dim]++;
                break;
            }else {
                // Else : check if this is the last coordinate
                if(curr_dim == 0) {
                    // If it is, we are trying to increment last index past the end: we have finished enumerating indexes
                    // end outer loop
                    return false;
                } else {
                    // If it isn't, carry increment to next coordinate
                    indexes[curr_dim] = 0;
                }
            }
        }
        return true;
    }
    */

}

#endif //FESUTILS_GRID_UTILS_HPP
