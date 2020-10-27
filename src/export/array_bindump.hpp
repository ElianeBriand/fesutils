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

#ifndef FESUTILS_ARRAY_BINDUMP_HPP
#define FESUTILS_ARRAY_BINDUMP_HPP

#include <boost/filesystem.hpp>
namespace bf = boost::filesystem;

namespace fesutils {

    /** Check if an adequate bindump file exists for the given text file path
     *
     * This check for existence, but also format of the file (header, version), as well as the
     * original file size as recorded in the header (simple heuristic for file change)
     *
     * \param path
     * \return
     */
    std::optional<bf::path> corresponding_bindump_exists(const bf::path& path);

}

#endif //FESUTILS_ARRAY_BINDUMP_HPP
