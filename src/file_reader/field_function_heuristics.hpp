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
