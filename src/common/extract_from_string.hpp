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
     * [SLOW] : do not use in main path
     *
     * \param str comma separated float values
     * \return vector of extracted values
     */
    std::vector<float> extract_range_from_string(const std::string& str);

    /** Split a line into space separated tokens.
     *
     * [SLOW] : do not use in main path
     *
     * \param line line to tokenize
     * \return vector of tokens
     */
    std::vector<std::string> slow_tokenize(const std::string& line);

    /**
     * Parse "false" and "true" to bool
     *
     * String should already be trimmed. eg: "false " will make it throw.
     *
     * [SLOW] : do not use in main path
     *
     * \param s value to parse
     * \return
     */
    bool parse_bool_as_word(const std::string& s);

}


#endif //FESUTILS_EXTRACT_FROM_STRING_HPP
