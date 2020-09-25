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

#ifndef FESUTILS_FIND_IN_SEARCH_DIRECTORY_HPP
#define FESUTILS_FIND_IN_SEARCH_DIRECTORY_HPP

#include <boost/filesystem.hpp>

#include "../GeneralOptions.h"


namespace bf = boost::filesystem;

namespace fesutils {

    bf::path find_cv_path_from_hint(const GeneralOptions& options, const std::string& cv_filepath_hint);

    bf::path find_hills_path_from_hint(const GeneralOptions& options, const std::string& hills_filepath_hint);

    bf::path find_grid_path_from_hint(const GeneralOptions& options, const std::string& grid_filepath_hint);

    bf::path find_any_path_from_hint(const GeneralOptions& options, const std::string& any_filepath_hint);

    std::optional<bf::path> find_path_from_hint_in_search_directory_category(const GeneralOptions& options,
                                                              const std::string& search_dir_cat,
                                                              const std::string& any_filepath_hint);

    bf::path find_path_from_hint_in_search_directory_categories(const GeneralOptions& options,
                                                                             const std::vector<std::string>& search_dir_cats,
                                                                             const std::string& any_filepath_hint);


}

#endif //FESUTILS_FIND_IN_SEARCH_DIRECTORY_HPP
