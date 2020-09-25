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

#ifndef FESUTILS_GRID_FILLER_NODE_HPP
#define FESUTILS_GRID_FILLER_NODE_HPP

#include <memory>
#include <vector>
#include <Eigen/Dense>
#include <tbb/flow_graph.h>

#include "GridData.hpp"


namespace fesutils {
    class grid_filler_node {
    public:

        explicit grid_filler_node(std::shared_ptr<GridData> grid_,
                                  unsigned int num_axis,
                                  unsigned int grid_value_idx_,
                                  std::vector<size_t> grid_variable_idx_);

        unsigned int operator()(std::shared_ptr<Eigen::Array<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor>> data);

    protected:
        std::shared_ptr<GridData> grid;
        unsigned int num_axis;
        unsigned int grid_value_idx;
        std::vector<size_t> grid_variable_idx;

    private:
        std::vector<double> coord_buffer;
        std::vector<long int> indexes_buffer;

    };
}



#endif //FESUTILS_GRID_FILLER_NODE_HPP
