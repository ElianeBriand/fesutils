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

#include <fstream>

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

    bool Grid::dump_binrepr_to_file(const std::string& filename) {
        std::ofstream outfile(filename, std::ios::out | std::ios::binary | std::ios::trunc);
        if(outfile.is_open()) {
            outfile.write(reinterpret_cast<char*>(&(this->num_dims)), sizeof(this->num_dims));
            size_t total_length = 1;
            for(size_t i = 0; i < this->num_dims; i++) {
                long int dim_length = this->dims.at(i);
                total_length *= dim_length;
                outfile.write(reinterpret_cast<char*>(&dim_length), sizeof(dim_length));
            }
            outfile.write(reinterpret_cast<const char*>(this->grid_array->get_raw_data_ptr()), total_length * sizeof(double));
            outfile.close();
            return true;
        }
        else {
            BOOST_LOG_TRIVIAL(error) << "Cannot open file " << filename << "for dumping grid data.";
            return false;
        }
        return false;
    }

    bool Grid::load_binrepr_from_file(const std::string& filename) {
        std::ifstream infile(filename, std::ios::in | std::ios::binary);
        if(infile.is_open()) {
            size_t dims_count = 0;
            infile.read(reinterpret_cast<char*>(&dims_count), sizeof(dims_count));
            std::vector<long int> dimensions_size;
            for(size_t i = 0; i < dims_count; i++) {
                long int curr_dim = 0;
                infile.read(reinterpret_cast<char*>(&curr_dim), sizeof(curr_dim));
                dimensions_size.push_back(curr_dim);
            }
            if(dims_count != this->num_dims) {
                BOOST_LOG_TRIVIAL(error) << "Mismatching dimension in binary dump: expected" << this->num_dims << " but got " << dims_count;
                return false;
            }
            size_t total_length = 1;
            for(size_t i = 0; i < dims_count; i++) {
                if(dimensions_size.at(i) != this->dims.at(i)) {
                    BOOST_LOG_TRIVIAL(error) << "Mismatching dimension size in binary dump: expected" << this->dims.at(i) << " but got " << dimensions_size.at(i) << " at index " << i;
                    return false;
                }
                total_length *= dimensions_size.at(i);
            }
            char* data_ptr = this->grid_array->get_raw_data_ptr();
            infile.read(data_ptr, total_length *  sizeof(double));
            if(infile.fail()) {
                BOOST_LOG_TRIVIAL(error) << "Binary dump was not long enough: expected " << total_length *  sizeof(double) << " bytes";
                return false;
            }
            return true;
        }
        else {
            return false;
        }
        return false;
    }

    // LCOV_EXCL_STOP

}


