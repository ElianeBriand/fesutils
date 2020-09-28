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

#include "reconstruction_utils.hpp"
#include "../file_reader/read_file_header.hpp"

#include <boost/log/trivial.hpp>

namespace fesutils {
/*
    PlumedDatHeader grid_from_intersection_of_grids(const FragmentInfo& fragmentInfo, const std::vector<PlumedDatHeader>& headers) {

        std::vector<PlumedDatHeader> headers_rw = headers;

        if(headers.empty()) {
            BOOST_LOG_TRIVIAL(error) << "Grid intersection requested, but no header provided";
            throw std::runtime_error("Empty vector of PlumedDatHeader");
        }

        if(! all_header_have_same_fields(headers)) {
            BOOST_LOG_TRIVIAL(error) << "All FES file must have the same fields name for the same variable, in the same order";
            BOOST_LOG_TRIVIAL(error) << "You may have to rename fields in the header for them to match";
            throw std::runtime_error("FES files fields have different names");
        }

        PlumedDatHeader header_common = headers.front();
        remove_nongrid_fields(header_common);
        size_t num_fields = header_common.fields.size();

        header_common.originating_file_path = "<Generated from intersection of multiple grids>";

        // Find minimal volume covered by all FES files
        for(int i = 0; i < num_fields; i++) {
            double min_val = std::get<double>(header_common.fields[i].attributes.at("min"));
            double max_val = std::get<double>(header_common.fields[i].attributes.at("max"));
            int nbins = std::get<int>(header_common.fields[i].attributes.at("nbins"));
            bool periodic  = std::get<bool>(header_common.fields[i].attributes.at("periodic"));
            for(auto & header : headers_rw) {
                remove_nongrid_fields(header);
                double min_val_curr = std::get<double>(header.fields[i].attributes.at("min"));
                double max_val_curr = std::get<double>(header.fields[i].attributes.at("max"));
                int nbins_curr = std::get<int>(header.fields[i].attributes.at("nbins"));
                bool periodic_curr  = std::get<bool>(header.fields[i].attributes.at("periodic"));

                min_val = std::max(min_val_curr, min_val); // maximum of minimums
                max_val = std::min(max_val_curr, max_val); // minimum of maximums
                nbins = std::max(nbins_curr, nbins); // Highest resolution that makes sense is max of all res
                if(periodic != periodic_curr) {
                    BOOST_LOG_TRIVIAL(error) << "All FES file must have the same periodic attribute value for the same field.";
                    throw std::runtime_error("periodic attribute mismatch");
                }
            }
            header_common.fields[i].attributes.at("min") = min_val;
            header_common.fields[i].attributes.at("max") = max_val;
            header_common.fields[i].attributes.at("nbins") = nbins;
            header_common.fields[i].attributes.at("periodic") = periodic;
        }


        return header_common;
    }

    bool all_header_have_same_fields(const std::vector<PlumedDatHeader>& headers) {
        std::vector<std::string> field_names;
        for(const auto& field : headers.front().fields) {
            if(is_field_a_grid_dimension(field)) {
                field_names.push_back(field.name);
            }
        }

        for(auto it = headers.begin(); it != headers.end(); it++) {
            std::vector<size_t> grid_field_idxs =  find_fields_index_that_have_required_attribute_for_grid(*it);

            if(grid_field_idxs.size() != field_names.size()) {
                BOOST_LOG_TRIVIAL(error) << "Different number of grid dimension field, got " <<
                grid_field_idxs.size() << ", expected " <<  field_names.size();
                return false;
            }


            for(int i = 0; i < grid_field_idxs.size(); i++) {
                if(it->fields[grid_field_idxs[i]].name !=  field_names[i]) {
                    BOOST_LOG_TRIVIAL(error) << "Different grid dimension field name: got " <<
                    it->fields[grid_field_idxs[i]].name << ", expected " << field_names[i];
                    return false;
                }

            }
        }
        return true;
    }

 */
}