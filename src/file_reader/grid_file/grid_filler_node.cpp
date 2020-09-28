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

#include <boost/log/trivial.hpp>



#include "grid_filler_node.hpp"


fesutils::grid_filler_node::grid_filler_node(std::shared_ptr<GridData> grid_,
                                             unsigned int num_axis_,
                                             unsigned int grid_value_idx_,
                                             std::vector<size_t> grid_variable_idx_) :
        grid(std::move(grid_)),
        num_axis(num_axis_),
        grid_value_idx(grid_value_idx_),
        coord_buffer(this->num_axis),
        indexes_buffer(this->num_axis),
        grid_variable_idx(std::move(grid_variable_idx_))
{
        assert(this->grid_variable_idx.size() == this->num_axis);
        assert(this->indexes_buffer.size() == this->num_axis);
        assert(this->coord_buffer.size() == this->num_axis);
}

unsigned int fesutils::grid_filler_node::operator()(
        std::shared_ptr<Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> data) {

    for(size_t row_n = 0; row_n < data->rows(); row_n++) {
        //BOOST_LOG_TRIVIAL(debug) << "grid_filler_node::operator() row_n = " << row_n;
        for(size_t i = 0; i < this->grid->get_num_axis(); i++){
            this->coord_buffer[i] = (*data)(row_n, this->grid_variable_idx[i]);
        }
        const double& value = (*data)(row_n, this->grid_value_idx);
        this->grid->insert_at_coord_rangechecked( this->coord_buffer, value, this->indexes_buffer);

    }

    return data->rows();
}
