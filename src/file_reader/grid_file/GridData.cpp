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

#include "GridData.hpp"

#include <locale>
#include <tuple>
#include <fmt/format.h>

#include <boost/log/trivial.hpp>

namespace fesutils {
    GridData::GridData(unsigned int num_axis,
                       std::vector<unsigned int> bins_per_axis_,
                       std::vector<double> min_bin_value_per_axis_,
                       std::vector<double> max_bin_value_per_axis_) :
                       num_axis(num_axis),
                       bins_per_axis(std::move(bins_per_axis_)),
                       min_bin_value_per_axis(std::move(min_bin_value_per_axis_)),
                       max_bin_value_per_axis(std::move(max_bin_value_per_axis_)),
                       track_write_access_number(false){

        if(this->bins_per_axis.size() != num_axis ||
            this->min_bin_value_per_axis.size() != num_axis ||
            this->max_bin_value_per_axis.size() != num_axis) {
            BOOST_LOG_TRIVIAL(error) << "GridData constructor called with inconsistent number of axis";
            BOOST_LOG_TRIVIAL(error) << "num_axis = " << num_axis
            << " but vector sizes are: " << this->bins_per_axis.size() << ", "
            << this->min_bin_value_per_axis.size() << ", "
            << this->max_bin_value_per_axis.size() << ".";
            throw std::runtime_error("GridData constructor error");
        }

        this->num_voxels = 1;
        for(const unsigned int& num_bins: this->bins_per_axis) {
            if(num_bins == 1) {
                BOOST_LOG_TRIVIAL(error) << "GridData does not support axis with only one bin.";
                throw std::runtime_error("Single-bin axis in GridData");
            }
            this->num_voxels *= num_bins;
        }

        for(unsigned int axis = 0; axis < num_axis; axis++) {
            const double range_between_maxmin_bins_value =
                    this->max_bin_value_per_axis[axis] - this->min_bin_value_per_axis[axis];
            const double bin_width = range_between_maxmin_bins_value/(this->bins_per_axis[axis] - 1);
            this->bin_width_per_axis.push_back(bin_width);
            if(bin_width < 0.0001) {
                BOOST_LOG_TRIVIAL(warning) << "Trying to construct a grid with a numerically very small bin width (" << bin_width << ")";
                BOOST_LOG_TRIVIAL(warning) << "This may have unforeseen consequences!";
                BOOST_LOG_TRIVIAL(warning) << "Axis " << axis << " of " <<  num_axis << " with bin width = "<< bin_width;
                this->has_small_bin_width_ = true;
            }
            const double min_bin_edge = this->min_bin_value_per_axis[axis] - (bin_width/2);
            this->bin_edges_per_axis.emplace_back(this->bins_per_axis[axis] + 1, 0.0);
            this->bin_centers_per_axis.emplace_back(this->bins_per_axis[axis], 0.0);

            // Start from lowest edge, increment by bin_width. N bins -> N+1 edges
            for(unsigned int i = 0; i <= this->bins_per_axis[axis]; i++) {
                this->bin_edges_per_axis[axis][i] = (min_bin_edge + i * bin_width);
            }

            // N bins, N centers
            const double bin_centers_distance = bin_width;
            for(unsigned int i = 0; i < this->bins_per_axis[axis]; i++) {
                this->bin_centers_per_axis[axis][i] = (this->min_bin_value_per_axis[axis] + i * bin_centers_distance);
            }

            this->axis_range_minmax.emplace_back(bin_edges_per_axis[axis].front(),bin_edges_per_axis[axis].back());

        }


    }


    const unsigned int& GridData::get_num_axis() {
        return this->num_axis;
    }

    const unsigned int& GridData::get_num_voxels() {
        return this->num_voxels;
    }

    const std::vector<unsigned int>& GridData::get_bins_per_axis() {
        return this->bins_per_axis;
    }

    const std::vector<double>& GridData::get_min_bin_value_per_axis() {
        return this->min_bin_value_per_axis;
    }

    const std::vector<double>& GridData::get_max_bin_value_per_axis() {
        return this->max_bin_value_per_axis;
    }

    const std::vector<double>& GridData::get_bin_width_per_axis() {
        return this->bin_width_per_axis;
    }

    const std::vector<std::vector<double>>& GridData::get_bin_edges_per_axis() {
        return this->bin_edges_per_axis;
    }

    const std::vector<std::vector<double>>& GridData::get_bin_centers_per_axis() {
        return this->bin_centers_per_axis;
    }


    InMemoryGridData::InMemoryGridData(unsigned int num_axis,
                                       std::vector<unsigned int> bins_per_axis,
                                       std::vector<double> min_bin_value_per_axis,
                                       std::vector<double> max_bin_value_per_axis)
            : GridData(num_axis,
                       std::move(bins_per_axis),
                       std::move(min_bin_value_per_axis),
                       std::move(max_bin_value_per_axis)),
                       grid_values(this->num_voxels, 0.0){
        BOOST_LOG_TRIVIAL(info) << fmt::format(
                "InMemoryGridData: Allocating  {:} bytes for grid ({:} voxels total)",
                (this->num_voxels * sizeof(double)), this->num_voxels );


    }



    bool GridData::coord_to_indices_rangechecked(const std::vector<double>& coord, std::vector<long int>& idx_buffer)  {
        for(unsigned int axis = 0; axis < this->num_axis; axis++) {
            const auto [range_min, range_max] = this->axis_range_minmax[axis];
            if(coord[axis] < range_min || coord[axis] > range_max) {
                return false;
            }
            const double abs_coord = coord[axis] - range_min;
            const double continuous_bin_index =  abs_coord / bin_width_per_axis[axis];
            const long int bin_index = std::floor(continuous_bin_index);
            idx_buffer[axis] = bin_index;
        }
        return true;
    }

    long int GridData::indices_to_globalindex(const std::vector<long int>& indices) {
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

    const bool& GridData::has_small_bin_width() {
        return this->has_small_bin_width_;
    }

    bool InMemoryGridData::insert_at_coord_rangechecked(const std::vector<double>& coord,
                                                        double value,
                                                        std::vector<long int>& idx_buffer) {
        const bool in_range = this->coord_to_indices_rangechecked(coord, idx_buffer);
        if(!in_range) {
            return false;
        }
        const long int global_index = this->indices_to_globalindex(idx_buffer);
        this->grid_values.at(global_index) = value;
        if(this->track_write_access_number) {
            this->grid_access_tracker->increment_at_coord_rangechecked(coord,idx_buffer);
        }
        return true;
    }

    bool InMemoryGridData::get_value_at_coord_rangechecked(const std::vector<double>& coord, double& value,
                                                           std::vector<long>& idx_buffer) {
        const bool in_range = this->coord_to_indices_rangechecked(coord, idx_buffer);
        if(!in_range) {
            return false;
        }
        const long int global_index = this->indices_to_globalindex(idx_buffer);
        value = this->grid_values.at(global_index);
        return true;
    }

    bool GridData::parallel_grid_write_posthoc_check() {
        return grid_access_tracker->all_voxel_accessed_n_times_or_less(1);
    }

    void GridData::enable_parallel_write_check() {
        this->track_write_access_number = true;
        this->grid_access_tracker = std::shared_ptr<GridAccessTracker>(new GridAccessTracker(this));
    }

    std::shared_ptr<GridData> GridData_factory(GridData_storage_class grid_storage_class,
                                               unsigned int num_axis,
                                             std::vector<unsigned int> bins_per_axis,
                                             std::vector<double> min_bin_value_per_axis,
                                             std::vector<double> max_bin_value_per_axis,
                                             bool track_write_access_number) {
        std::shared_ptr<GridData> new_griddata_object;

        if(grid_storage_class == fesutils::GridData_storage_class::inmemory) {
            new_griddata_object = std::make_shared<InMemoryGridData>(num_axis,
                                                                     std::move(bins_per_axis),
                                                                     std::move(min_bin_value_per_axis),
                                                                     std::move(max_bin_value_per_axis));
            if(track_write_access_number) {
                new_griddata_object->enable_parallel_write_check();
            }
        } else {
            // LCOV_EXCL_START
            // Reason for coverage exclusion: Difficult to generate incorrect values for enum class
            BOOST_LOG_TRIVIAL(error) << "Unknown GRID storage class enum value";
            throw std::runtime_error("Unknown GRID storage class");
            // LCOV_EXCL_STOP
        }

        return new_griddata_object;
    }
}


