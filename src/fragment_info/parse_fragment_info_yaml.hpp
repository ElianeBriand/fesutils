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

#ifndef FESUTILS_PARSE_FRAGMENT_INFO_YAML_HPP
#define FESUTILS_PARSE_FRAGMENT_INFO_YAML_HPP

#include <string>

#include "FragmentInfo.hpp"

namespace fesutils {

    FragmentInfo parse_fragment_info_yaml(const std::string& fragmentinfo_yaml_path, const std::vector<std::string>& fes_paths);


}

#endif //FESUTILS_PARSE_FRAGMENT_INFO_YAML_HPP