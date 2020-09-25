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

#include "CVData_filler_node.hpp"

#include <utility>


namespace fesutils {

    CVData_filler_node::CVData_filler_node(std::shared_ptr<CVData> CVData_container_,
                                           std::vector<size_t>  cv_vars_idx_, const size_t& metad_bias_var_idx_,
                                           std::vector<size_t>  other_vars_idx_):
                                           CVData_container(std::move(CVData_container_)),
                                           cv_vars_idx(std::move(cv_vars_idx_)),
                                           metad_bias_var_idx(metad_bias_var_idx_),
                                           other_vars_idx(std::move(other_vars_idx_))
                                           {

    }

    tbb::flow::continue_msg CVData_filler_node::operator()(
            std::shared_ptr<Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> data) {

        cv_data_buffer.reserve(this->CVData_container->get_total_dim_in_one_record() * data->rows());


        for(size_t row_n = 0; row_n < data->rows(); row_n++) {
            for(const size_t& cv_var_idx: this->cv_vars_idx) {
                cv_data_buffer.push_back( (*data)(row_n, cv_var_idx) );
            }
            cv_data_buffer.push_back((*data)(row_n, this->metad_bias_var_idx));
            for(const size_t& other_var_idx: this->other_vars_idx) {
                cv_data_buffer.push_back( (*data)(row_n, other_var_idx) );
            }
        }

        this->CVData_container->insertDatapointsVector(this->cv_data_buffer);

        this->cv_data_buffer.clear();

        return tbb::flow::continue_msg();
    }
}