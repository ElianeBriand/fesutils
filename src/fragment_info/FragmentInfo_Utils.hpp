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

#ifndef FESUTILS_FRAGMENTINFO_UTILS_HPP
#define FESUTILS_FRAGMENTINFO_UTILS_HPP

#include "FragmentInfo.hpp"

namespace fesutils {

    void fragment_path_from_filename(FragmentInfo& finfo, const std::vector<std::string>& fes_paths);
}

#endif //FESUTILS_FRAGMENTINFO_UTILS_HPP
