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

#include "grid_utils.hpp"

#include <boost/log/trivial.hpp>


std::vector<long int> fesutils::grid_dims_from_header(const fesutils::PlumedDatHeader& header, const std::vector<size_t>& which_fields) {
    std::vector<long int> dims;

    for(const size_t& field_idx: which_fields) {
        const Field field = header.fields[field_idx];
        const int nbins = std::get<int>(field.attributes.at("nbins"));
        if(nbins <= 0) {
            throw std::runtime_error("Encountered nbins value <= 0 (nbins=" + std::to_string(nbins)+") - Error code 00004");
        }
        dims.push_back(nbins);
    }

    return dims;
}


std::vector<size_t> fesutils::find_fields_index_that_have_required_attribute_for_grid(const fesutils::PlumedDatHeader& header) {
    std::vector<size_t> indexes;
    for(int i = 0; i < header.fields.size(); i++) {
        const Field field = header.fields[i];
        if( !field.attributes.count("min") ||
        !field.attributes.count("max") ||
        !field.attributes.count("nbins") ||
        !field.attributes.count("periodic")) {
            // Missing information to make the bins
        }else {
            indexes.push_back(i);
        }
    }
    return indexes;
}

std::vector<double> fesutils::grid_bin_sizes_from_header(const fesutils::PlumedDatHeader& header, const std::vector<size_t>& which_fields) {
    std::vector<double> bin_sizes;

    for(const size_t& field_idx: which_fields) {
        const Field field = header.fields[field_idx];
        const int nbins = std::get<int>(field.attributes.at("nbins"));
        if(nbins <= 0) {
            throw std::runtime_error("Encountered nbins value <= 0 (nbins=" + std::to_string(nbins)+") - Error code 00004");
        }
        const double min_val = std::get<double>(field.attributes.at("min"));
        const double max_val = std::get<double>(field.attributes.at("max"));
        const double bin_size = (max_val - min_val) / nbins;
        bin_sizes.push_back(bin_size);
    }

    return bin_sizes;
}

std::vector<double>
fesutils::extract_grid_min_val(const fesutils::PlumedDatHeader& header, const std::vector<size_t>& which_fields) {
    std::vector<double> min_vals;

    for(const size_t& field_idx: which_fields) {
        const Field field = header.fields[field_idx];
        const int min = std::get<double>(field.attributes.at("min"));
        min_vals.push_back(min);
    }
    return min_vals;
}

std::vector<double>
fesutils::extract_grid_max_val(const fesutils::PlumedDatHeader& header, const std::vector<size_t>& which_fields) {
    std::vector<double> min_vals;

    for(const size_t& field_idx: which_fields) {
        const Field field = header.fields[field_idx];
        const int min = std::get<double>(field.attributes.at("max"));
        min_vals.push_back(min);
    }
    return min_vals;
}

std::vector<std::vector<double>>
fesutils::compute_grid_bin_edges(const std::vector<double>& min_vals,
                                const std::vector<double>& max_vals,
                                 const std::vector<double>& bin_sizes,
                                 const std::vector<long int>& dims) {
    std::vector<std::vector<double>> bin_edges_alldims;
    for(int i = 0; i < dims.size(); i++) {
        const size_t dim_size = dims[i];
        const double dim_max_val = max_vals[i];
        const double dim_min_val = min_vals[i];
        const double bin_size = bin_sizes[i];
        std::vector<double> edges;
        for(size_t j = 0; j < dim_size; j++) {
            edges.push_back(dim_min_val + j*bin_size);
        }
        edges.push_back(dim_max_val);

        // LCOV_EXCL_START
        // Reason for coverage exclusion: ought to be unreachable.
        if(edges.size() < 2) {
            BOOST_LOG_TRIVIAL(error) << "Computed " << edges.size() << " bin edges for dimension #"<< i << " but 2 edges is the minimum logically possible.";
            BOOST_LOG_TRIVIAL(error) << "Check nbins in the header";
            throw std::runtime_error("Computed less than 2 bin edges for the grid");
        }
        // LCOV_EXCL_STOP

        bin_edges_alldims.push_back(std::move(edges));
    }
    return bin_edges_alldims;
}

std::vector<std::vector<double>> fesutils::compute_grid_bin_center(const std::vector<std::vector<double>>& bin_edges) {
    std::vector<std::vector<double>> bin_centers_all_dims;
    for(int i = 0; i < bin_edges.size(); i++) {
        const std::vector<double>& bins = bin_edges[i];
        std::vector<double> bin_centers;
        for(int j = 1; j < bins.size(); j++) {
            const double bin_center = (bins[j] + bins[j-1])/2;
            bin_centers.push_back(bin_center);
        }

        // LCOV_EXCL_START
        // Reason for coverage exclusion: ought to be unreachable.
        if(bin_centers.size() < 1) {
            BOOST_LOG_TRIVIAL(error) << "Computed " << bin_centers.size() << " bin center for dimension #"<< i << " but 1 center is the minimum logically possible.";
            BOOST_LOG_TRIVIAL(error) << "Check nbins in the header";
            throw std::runtime_error("Computed less than 1 bin center for the grid");
        }
        // LCOV_EXCL_STOP

        bin_centers_all_dims.push_back(std::move(bin_centers));
    }
    return bin_centers_all_dims;
}

std::optional<size_t> fesutils::find_likely_bias_field_index(const fesutils::PlumedDatHeader& header) {
    std::optional<size_t> ret;

    size_t number_of_hit = std::count_if(header.fields.begin(), header.fields.end(), [](const Field& f) {
        return f.name.find(".bias") != std::string::npos; // Find field that has bias
    });

    if(number_of_hit == 0 || number_of_hit > 1) {
        // No matching field, or more than 1: can't guess
        return ret;
    } else { // (number_of_hit == 1)
        // Exactly one matching field: likely to be bias

        auto it = std::find_if(header.fields.begin(), header.fields.end(), [](const Field& f) {
            return f.name.find(".bias") != std::string::npos; // Find field that has bias
        });

        return std::optional<size_t>(std::distance(header.fields.begin(), it));
    }
}
