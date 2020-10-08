//
// Created by eliane on 25/05/2020.
//

#include "field_function_heuristics.hpp"

#include <algorithm>
#include <boost/log/trivial.hpp>

#include "PlumedDatHeader.hpp"


namespace fesutils {
    std::optional<size_t> find_likely_bias_field_index(const PlumedDatHeader& header) {
        BOOST_LOG_TRIVIAL(debug) << "DEPRECATED call to find_likely_bias_field_index";
        return find_unique_field_name_fragment_index(header, ".bias");
    }

    std::optional<size_t> find_likely_fenerg_field_index(const PlumedDatHeader& header) {
        BOOST_LOG_TRIVIAL(debug) << "DEPRECATED call to find_likely_fenerg_field_index";
        return find_unique_field_name_fragment_index(header, "ff");
    }


    std::optional<size_t> find_unique_field_name_fragment_index(const PlumedDatHeader& header, const std::string& field_name_fragment) {
        std::optional<size_t> ret;
        BOOST_LOG_TRIVIAL(debug) << "DEPRECATED call to find_unique_field_name_fragment_index";

        size_t number_of_hit = std::count_if(header.fields.begin(), header.fields.end(), [&](const Field& f) {
            return f.name.find(field_name_fragment) != std::string::npos; // Find field that has this name
        });

        if(number_of_hit == 0 || number_of_hit > 1) {
            // No matching field, or more than 1: can't guess

            return ret;
        } else { // (number_of_hit == 1)
            // Exactly one matching field: likely to be bias

            auto it = std::find_if(header.fields.begin(), header.fields.end(), [&](const Field& f) {
                return f.name.find(field_name_fragment) != std::string::npos;
            });

            return std::optional<size_t>(std::distance(header.fields.begin(), it));
        }
    }


}