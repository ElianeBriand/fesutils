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


#ifndef FESUTILS_RUNFILE_SEARCH_DIRECTORY_SECTION_HPP
#define FESUTILS_RUNFILE_SEARCH_DIRECTORY_SECTION_HPP

#include "../GeneralOptions.h"

#include <yaml-cpp/yaml.h>

namespace fesutils {
    void parse_search_directory_section(GeneralOptions& options, YAML::Node& search_directory_node);

}

#endif //FESUTILS_RUNFILE_SEARCH_DIRECTORY_SECTION_HPP
