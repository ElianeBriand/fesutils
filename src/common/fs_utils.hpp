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

#ifndef FESUTILS_FS_UTILS_HPP
#define FESUTILS_FS_UTILS_HPP

#include <string>

namespace fesutils {

    /**
     * Check if a path exist, and is a normal file
     * \param filepath path to check
     * \return true if the file exists and is a normal file
     */
    bool path_exists_isfile(const std::string& filepath);

    /** Return an error message for why the file could not be opened.
     *
     * PRECONDITION: path_exists_isfile called on filepath, returned false
     *
     * \param filepath path to check
     * \return a string containing the error message
     * \sa path_exists_isfile
     */
    std::string path_notfile_whatistheproblem(const std::string& filepath);

}

#endif //FESUTILS_FS_UTILS_HPP
