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

#ifndef FESUTILS_PARSE_SPACE_SEPARATED_VALUES_NODE_HPP
#define FESUTILS_PARSE_SPACE_SEPARATED_VALUES_NODE_HPP


#include <vector>
#include <string>
#include <memory>
#include <Eigen/Dense>

namespace fesutils {

    class parse_space_separated_double_node {
    public:

        explicit parse_space_separated_double_node(size_t num_cols);

        std::shared_ptr<Eigen::Array<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor>>
            operator()(const std::shared_ptr<std::vector<std::string>> lines_sptr);

    protected:

        size_t num_columns;

        size_t processed_count;
        size_t error_count;

        size_t dbg_call_to_operator;
    };
}



#endif //FESUTILS_PARSE_SPACE_SEPARATED_VALUES_NODE_HPP
