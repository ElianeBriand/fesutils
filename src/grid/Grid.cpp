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


#include "Grid.hpp"

#include <boost/log/trivial.hpp>



#include "grid_utils.hpp"

namespace fesutils {

    Grid::Grid(const PlumedDatHeader& header) :
            header_copy(header){

        this->header_grid_field_indexes = find_fields_index_that_have_required_attribute_for_grid(header);

        if(this->header_grid_field_indexes.size() == 0) {
            BOOST_LOG_TRIVIAL(error) << "The .dat file header does not contain enough information to create an empty grid.";
            BOOST_LOG_TRIVIAL(error) << "Error code: 00001";
            throw std::runtime_error("dat file header imcomplete");
        }

        this->dims = grid_dims_from_header(header, this->header_grid_field_indexes);
        this->num_dims = this->dims.size();
        this->min_vals = extract_grid_min_val(header, this->header_grid_field_indexes);
        this->max_vals = extract_grid_max_val(header, this->header_grid_field_indexes);
        this->bin_sizes = grid_bin_sizes_from_header(header, this->header_grid_field_indexes);
        this->bin_edges = compute_grid_bin_edges(this->min_vals,this->max_vals, this->bin_sizes, this->dims);
        this->bin_center = compute_grid_bin_center(this->bin_edges);

        this->grid_array = NDArray_factory<double>(this->dims);
        this->grid_array->zeroOut();

    }

    // LCOV_EXCL_START
    // Reason for coverage exclusion: purely output printing

    std::ostream& operator<<(std::ostream& os, const fesutils::Grid& grid) {
        os << "Grid information: " << "\n";
        os << "Num dims:  " << std::to_string(grid.dims.size()) << "\n";
        os << "Dims    : [" ;
        for(const size_t dim: grid.dims) {
            os << std::to_string(dim)<< ", ";
        }
        os << "]\n" ;
        os << "Bin sizes    : " ;
        for(const double bins_size: grid.bin_sizes) {
            os << std::to_string(bins_size)<< ", ";
        }
        os << "\n" ;

        return os;
    }

    // LCOV_EXCL_STOP

}


