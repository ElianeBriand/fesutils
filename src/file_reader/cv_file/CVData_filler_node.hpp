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

#ifndef FESUTILS_CVDATA_FILLER_NODE_HPP
#define FESUTILS_CVDATA_FILLER_NODE_HPP

#include <tbb/flow_graph.h>

#include <Eigen/Dense>

#include "CVData.hpp"

namespace fesutils {


    class CVData_filler_node {
    public:

        explicit CVData_filler_node(std::shared_ptr<CVData> CVData_container_,
                                    std::vector<size_t>  cv_vars_idx_,
                                    const size_t& metad_bias_var_idx_,
                                    std::vector<size_t>  other_vars_idx_);

        tbb::flow::continue_msg operator()(std::shared_ptr<Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> data);

    protected:
        std::shared_ptr<CVData> CVData_container;
        std::vector<size_t> cv_vars_idx;
        size_t metad_bias_var_idx;
        std::vector<size_t> other_vars_idx;

    private:
        std::vector<double> cv_data_buffer;
    };


}




#endif //FESUTILS_CVDATA_FILLER_NODE_HPP
