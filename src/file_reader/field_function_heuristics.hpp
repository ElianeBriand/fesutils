//
// Created by eliane on 25/05/2020.
//

#ifndef FESUTILS_FIELD_FUNCTION_HEURISTICS_HPP
#define FESUTILS_FIELD_FUNCTION_HEURISTICS_HPP

#include <optional>

#include "PlumedDatHeader.hpp"

namespace fesutils {

    /** Identify index of which field is likely to be the bias
    */
    std::optional<size_t> find_likely_bias_field_index(const PlumedDatHeader& header);

    /** Identify index of which field is likely to be the free energy
    */
    std::optional<size_t> find_likely_fenerg_field_index(const PlumedDatHeader& header);

    /** Identify index of which field contains the name fragment passed as argument, if there is only one
    */
    std::optional<size_t> find_unique_field_name_fragment_index(const PlumedDatHeader& header, const std::string& field_name_fragment);

}

#endif //FESUTILS_FIELD_FUNCTION_HEURISTICS_HPP
