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

#ifndef FESUTILS_EXTRACT_FROM_STRING_HPP
#define FESUTILS_EXTRACT_FROM_STRING_HPP

#include <vector>
#include <string>

namespace fesutils {

    /**
     * Extract comma-separated float value from string
     *
     * \param str comma separated float values
     * \return vector of extracted values
     */
    std::vector<float> extract_range_from_string(const std::string& str);

}


#endif //FESUTILS_EXTRACT_FROM_STRING_HPP
