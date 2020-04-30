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

#ifndef FESUTILS_GRID_HPP
#define FESUTILS_GRID_HPP

#include "../file_reader/PlumedDatHeader.hpp"
#include "./NDArray.hpp"

#include <cmath>

namespace fesutils {


    struct Grid {

        explicit Grid(const PlumedDatHeader& header);

        friend std::ostream& operator<<(std::ostream& os, const Grid& grid);

        bool dump_binrepr_to_file(const std::string& filename);
        bool load_binrepr_from_file(const std::string& filename);

        const PlumedDatHeader header_copy;
        std::vector<size_t> header_grid_field_indexes;

        size_t num_dims;
        std::vector<long int> dims;
        std::vector<double> min_vals;
        std::vector<double> max_vals;
        std::vector<double> bin_sizes;
        std::vector<std::vector<double>> bin_edges;
        std::vector<std::vector<double>> bin_center;

        std::shared_ptr<NDArray<double>> grid_array;


        /** Insert value into the grid at given coordinate, if the coordinate are garantueed to lie in the grid. With coordinate buffer.
         *
         * If the coordinate may not lie in the grid, used insert_reject_out_of_range (which rejects gracefully coordinate outside the grid).
         * This version uses a reference to a std::vector<int> as a buffer for coordinate. If you are doing many insert, this
         * saves a (somewhat costly) vector allocation. No modification of this buffer allowed while the function is running.
         *
         * PRECONDITION: indexes_buffer.size() == this->dims.size() && indexes_buffer.size() == coordinate.size()
         *
         * \param coordinate coordinate of the value (will be converted to the bin/voxel)
         * \param value value to insert instead of the current value
         * \param indexes_buffer
         * \sa insert_reject_out_of_range
         */
        inline void set_inrange(const std::vector<double>& coordinate, const double& value, std::vector<long int>& indexes_buffer) {
            for(int i = 0; i < this->num_dims; i++) {
                const int index = std::floor((coordinate[i] - this->min_vals[i])/this->bin_sizes[i]);
                indexes_buffer[i] = index;
            }
            this->grid_array->set(indexes_buffer, value);
        }

        /** Insert value into the grid at given coordinate, if the coordinate are garantueed to lie in the grid. Without coordinate buffer.
         *
         * If the coordinate may not lie in the grid, used insert_reject_out_of_range (which rejects gracefully coordinate
         * outside the grid). If you are doing many insert, you may want to use the overload with a coordinate buffer to
         * save an allocation.
         *
         * \param coordinate coordinate of the value (will be converted to the bin/voxel)
         * \param value value to insert instead of the current value
         * \sa insert_reject_out_of_range
         */
        inline void set_inrange(const std::vector<double>& coordinate, const double& value) {
            std::vector<long int> indexes(this->num_dims);
            this->set_inrange(coordinate, value, indexes);
        }

        /** Check if coordinate are in the grid
         *
         * PRECONDITION: the coordinate vector has the appropriate number of dimension.
         *
         * The caller is reponsible for the precondition: it can be checked using this->num_dims
         *
         * \param coordinate vector of coordinate
         * \return whether the coordinate lie in range
         */
        inline bool check_inrange(const std::vector<double>& coordinate) {
            for(int i = 0; i < this->num_dims; i++) {
                if (coordinate[i] < this->min_vals[i] || coordinate[i] > this->max_vals[i]) {
                    return false;
                }
            }
            return true;
        }


        /** Insert value into the grid at given coordinate, or do nothing if it does not lie in the grid. With coordinate buffer.
         *
         * This version uses a reference to a std::vector<int> as a buffer for coordinate. If you are doing many insert, this
         * saves a (somewhat costly) vector allocation. No modification of this buffer allowed while the function is running.
         * If the value does not lie in the grid, returns early without changing the grid.
         *
         * PRECONDITION: indexes_buffer.size() == this->dims.size() && indexes_buffer.size() == coordinate.size()
         * 
         * \param coordinate coordinate of the value (will be converted to the bin/voxel)
         * \param value value to insert instead of the current value
         * \param indexes_buffer
         * \sa insert_reject_out_of_range
         */
        inline void set_reject_out_of_range(const std::vector<double>& coordinate, const double& value, std::vector<long int>& indexes_buffer) {
            for(int i = 0; i < this->num_dims; i++) {
                if (coordinate[i] < this->min_vals[i] || coordinate[i] > this->max_vals[i]) {
                    return;
                }
                const int index = std::floor((coordinate[i] - this->min_vals[i])/this->bin_sizes[i]);
                indexes_buffer[i] = index;
            }
            this->grid_array->set(indexes_buffer, value);
        }

        /** Insert value into the grid at given coordinate, or do nothing if it does not lie in the grid. Without coordinate buffer.
         *
         * If you are doing many insert, you may want to use the overload with a coordinate buffer to
         * save an allocation. If the value does not lie in the grid, returns early without changing the grid.
         *
         * \param coordinate coordinate of the value (will be converted to the bin/voxel)
         * \param value value to insert instead of the current value
         * \sa insert_reject_out_of_range
         */
        inline void set_reject_out_of_range(const std::vector<double>& coordinate, const double& value) {
            std::vector<long int> indexes(this->num_dims);
            this->set_reject_out_of_range(coordinate, value, indexes);
        }

        /** Get the value at the given coordinate. Coordinates must be in range for the grid
         *
         * This does not check if the coordinate are actually in the grid. If not, the behaviour is undefined.
         *
         * In this version, a indexes_buffer of appropriate dimension must be provided (this saves an allocation)
         *
         * \param coordinate
         * \param indexes_buffer
         * \return
         */
        inline double& get_inrange(const std::vector<double>& coordinate, std::vector<long int>& indexes_buffer) {
            for(int i = 0; i < this->num_dims; i++) {
                const int index = std::floor((coordinate[i] - this->min_vals[i])/this->bin_sizes[i]);
                indexes_buffer[i] = index;
            }
            return this->grid_array->operator[](indexes_buffer);
        }

        /** Get the value at the given coordinate. Coordinates must be in range for the grid
         *
         * This does not check if the coordinate are actually in the grid. If not, the behaviour is undefined.
         *
         * If many values are fetches, the overload with an indexes_buffer may be preferable.
         *
         * \param coordinate
         * \return
         */
        inline double& get_inrange(const std::vector<double>& coordinate) {
            std::vector<long int> indexes_buffer(this->num_dims);
            return this->get_inrange(coordinate, indexes_buffer);
        }


    };

    std::ostream& operator<<(std::ostream& os, const fesutils::Grid& grid);


}



#endif //FESUTILS_GRID_HPP
