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

#include "find_in_search_directory.hpp"

#include <boost/log/trivial.hpp>


namespace fesutils {

    bf::path find_cv_path_from_hint(const GeneralOptions& options, const std::string& cv_filepath_hint) {
        bf::path p;
        p =  find_path_from_hint_in_search_directory_categories(options,
                                                              {"cv", "all"},
                                                              cv_filepath_hint);

        return p;
    }

    bf::path find_hills_path_from_hint(const GeneralOptions& options, const std::string& hills_filepath_hint) {
        bf::path p;
        p =  find_path_from_hint_in_search_directory_categories(options,
                                                                {"hills", "all"},
                                                                hills_filepath_hint);

        return p;
    }

    bf::path find_grid_path_from_hint(const GeneralOptions& options, const std::string& grid_filepath_hint) {
        bf::path p;
        p =  find_path_from_hint_in_search_directory_categories(options,
                                                                {"grids", "all"},
                                                                grid_filepath_hint);

        return p;
    }

    bf::path find_any_path_from_hint(const GeneralOptions& options, const std::string& any_filepath_hint) {
        bf::path p;
        p =  find_path_from_hint_in_search_directory_categories(options,
                                                                {"all"},
                                                                any_filepath_hint);

        return p;
    }

    std::optional<bf::path> find_path_from_hint_in_search_directory_category(const GeneralOptions& options,
                                                                             const std::string& search_dir_cat,
                                                                             const std::string& any_filepath_hint) {
        std::optional<bf::path> maybe_p; // empty at construction
        if(options.search_directory.count(search_dir_cat)) {
            bf::path p = options.search_directory.at(search_dir_cat);
            p /= any_filepath_hint;
            if(bf::exists(p) && bf::is_regular(p)) {
                maybe_p = p;
            }
        }
        return maybe_p;

    }

    bf::path find_path_from_hint_in_search_directory_categories(const GeneralOptions& options,
                                                                               const std::vector<std::string>& search_dir_cats,
                                                                               const std::string& any_filepath_hint) {
        std::optional<bf::path> maybe_p;
        for(const std::string& search_dir_cat: search_dir_cats) {
            maybe_p =  find_path_from_hint_in_search_directory_category(options, search_dir_cat, any_filepath_hint);
            if(maybe_p)
                break;
        }
        if(!maybe_p) {
            BOOST_LOG_TRIVIAL(error) << "Error while searching for file with filename hint " << any_filepath_hint;
            BOOST_LOG_TRIVIAL(error) << "No such file could be found. (error code 00005)";
            throw std::runtime_error("No file corresponding to filename hint was found");
        }
        return maybe_p.value();
    }
}