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


fesutils::grid_filler_node::grid_filler_node(std::shared_ptr<Grid> grid_, size_t grid_value_idx_) :
        grid(grid_), grid_value_idx(grid_value_idx_)
{
    // Reserve and pre-fill coord & index buffer
    this->coord_buffer.reserve(this->grid->num_dims);
    this->indexes_buffer.reserve(this->grid->num_dims);
    for(size_t i = 0; i < this->grid->num_dims; i++) {
        this->coord_buffer.push_back(0.0);
        this->indexes_buffer.push_back(0);
    }

}

tbb::flow::continue_msg fesutils::grid_filler_node::operator()(
        std::shared_ptr<Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> data) {
#ifndef NDEBUG
    if(data->cols() < this->grid->num_dims + 1 ) {
        BOOST_LOG_TRIVIAL(error) << "Data packet with too few columns: expected " << this->grid->num_dims << "+1 but got " << data->cols();
        throw std::runtime_error("Data packet dimensions incompatible");
    }
    if(this->grid_value_idx >= data->cols()) {
        BOOST_LOG_TRIVIAL(error) << "Grid value index out-of-range: got " << this->grid_value_idx << " but data packet has " << data->cols() << " columns";
        throw std::runtime_error("Data packet dimensions incompatible");
    }
#endif
    for(size_t row_n = 0; row_n < data->rows(); row_n++) {
        //BOOST_LOG_TRIVIAL(debug) << "grid_filler_node::operator() row_n = " << row_n;
        for(size_t i = 0; i < this->grid->num_dims; i++){
            size_t coord_index_in_data = this->grid->header_grid_field_indexes[i];
            this->coord_buffer[i] = (*data)(row_n, coord_index_in_data);
            const double& value = (*data)(row_n, this->grid_value_idx);
            this->grid->set_reject_out_of_range( this->coord_buffer, value, this->indexes_buffer);
        }
    }

    return tbb::flow::continue_msg();
}
