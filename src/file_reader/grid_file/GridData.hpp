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


#ifndef FESUTILS_GRIDDATA_HPP
#define FESUTILS_GRIDDATA_HPP

#include <mutex>
#include <vector>
#include <memory>
#include "GridAccessTracker.hpp"

namespace fesutils {

    class GridAccessTracker;


    bool common_coord_to_indices_rangechecked(const std::vector<double>& coord,
                                              std::vector<long int>& idx_buffer,
                                              const unsigned int& num_axis,
                                              const std::vector<std::tuple<double, double>>& axis_range_minmax,
                                              const std::vector<double>& bin_width_per_axis);

    /** Abstract class describing the interface for the Grid storage facility.
     *
     * Plumed can output metadynamics grid - this class describe the interface to store and interact with them.
     *
     */
    class GridData {
    public:
        GridData(unsigned int num_axis,
                 std::vector<unsigned int> bins_per_axis,
                 std::vector<double> min_bin_value_per_axis,
                 std::vector<double> max_bin_value_per_axis);
        ~GridData() = default;

        virtual const unsigned int& get_num_axis() const;

        virtual const unsigned int& get_num_voxels() const;


        virtual const std::vector<unsigned int>& get_bins_per_axis() const;

        virtual const std::vector<double>& get_min_bin_value_per_axis() const;

        virtual const std::vector<double>& get_max_bin_value_per_axis() const;

        virtual const std::vector<std::tuple<double, double>>& get_minmax_range_per_axis() const;

        virtual const std::vector<double>& get_bin_width_per_axis() const;

        virtual const std::vector<std::vector<double>>& get_bin_edges_per_axis() const;

        virtual const std::vector<std::vector<double>>& get_bin_centers_per_axis() const;

        virtual const bool& has_small_bin_width() const;

        bool coord_to_indices_rangechecked(const std::vector<double>& coord, std::vector<long int>& idx_buffer);

        virtual bool insert_at_coord_rangechecked(const std::vector<double>& coord, double value, std::vector<long int>& idx_buffer) = 0;

        virtual bool get_value_at_coord_rangechecked(const std::vector<double>& coord, double& value, std::vector<long int>& idx_buffer) = 0;

        virtual void enable_parallel_write_check();
        virtual bool parallel_grid_write_posthoc_check();

        virtual std::shared_ptr<GridAccessTracker> get_GridAccessTracker();

    protected:
        long int indices_to_globalindex(const std::vector<long int>& indices);

        unsigned int num_axis;
        unsigned int num_voxels;
        std::vector<unsigned int> bins_per_axis;
        std::vector<double> min_bin_value_per_axis;
        std::vector<double> max_bin_value_per_axis;
        std::vector<double> bin_width_per_axis;
        std::vector<std::vector<double>> bin_edges_per_axis;
        std::vector<std::vector<double>> bin_centers_per_axis;

        std::vector<std::tuple<double, double>> axis_range_minmax;

        bool has_small_bin_width_ = false;

        bool track_write_access_number;
        std::shared_ptr<GridAccessTracker> grid_access_tracker;


    };

    /** GridData kept entirely in memory
     *
     */
    class InMemoryGridData : public GridData {
    public:
        InMemoryGridData(unsigned int num_axis,
                         std::vector<unsigned int> bins_per_axis,
                         std::vector<double> min_bin_value_per_axis,
                         std::vector<double> max_bin_value_per_axis);
        ~InMemoryGridData() = default;

        bool insert_at_coord_rangechecked(const std::vector<double>& coord, double value, std::vector<long int>& coord_buffer) final;

        bool get_value_at_coord_rangechecked(const std::vector<double>& coord, double& value, std::vector<long int>& idx_buffer) final;



    protected:
        std::vector<double> grid_values;

    };

    enum class GridData_storage_class {
        inmemory
    };

    /** Factory function to generate a GridData storage object.
     *
     * \param grid_storage_class
     * \param num_axis
     * \param bins_per_axis
     * \param min_bin_value_per_axis
     * \param max_bin_value_per_axis
     * \param track_write_access_number
     * \return
     */
    std::shared_ptr<GridData> GridData_factory(GridData_storage_class grid_storage_class,
                                             unsigned int num_axis,
                                             std::vector<unsigned int> bins_per_axis,
                                             std::vector<double> min_bin_value_per_axis,
                                             std::vector<double> max_bin_value_per_axis,
                                             bool track_write_access_number = false);

}

#endif //FESUTILS_GRIDDATA_HPP
