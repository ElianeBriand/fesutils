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

#ifndef FESUTILS_CONVERSIONS_HPP
#define FESUTILS_CONVERSIONS_HPP

#include <string>
#include <vector>

/** Converts a vector of uint8_t to a string.
 *
 * No unicode check or anything.
 *
 * \param buffer vector of byte
 * \return
 */
inline std::string buffer_to_string(const std::vector<uint8_t>& buffer) {
    return std::string((char*) buffer.data(), buffer.size());
}


#endif //FESUTILS_CONVERSIONS_HPP
