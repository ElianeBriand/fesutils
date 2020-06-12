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

#include "FragmentInfo_Utils.hpp"

#include <algorithm>
#include <boost/log/trivial.hpp>


namespace fesutils {


    void fragment_path_from_filename(FragmentInfo& finfo, const std::vector<std::string>& fes_paths) {
        for(FragmentInfo::Fragment& fragment : finfo.fragments) {
            const std::string& filename_hint = fragment.filename_hint;
            size_t number_of_hit = std::count_if(fes_paths.begin(), fes_paths.end(), [&](const std::string& path) {
                return path.find(filename_hint) != std::string::npos; // Find path that contains this filename
            });

            if(number_of_hit == 0 || number_of_hit > 1) {
                BOOST_LOG_TRIVIAL(error) << "Cannot find associated file for fragment " << fragment.name;
                BOOST_LOG_TRIVIAL(error) << "Filename hint: " << fragment.filename_hint;
                BOOST_LOG_TRIVIAL(error) << "Available paths: ";
                for(const std::string& path : fes_paths) {
                    BOOST_LOG_TRIVIAL(error) << "  - " << path;
                }
                if(number_of_hit == 0) {
                    BOOST_LOG_TRIVIAL(error) << "No path match this filename";
                } else {
                    BOOST_LOG_TRIVIAL(error) << "Multiple paths match this filename";
                }

                throw std::runtime_error("Cannot find path for given fragment filename");
            } else { // (number_of_hit == 1)
                // Exactly one matching field: likely to be the file

                auto it = std::find_if(fes_paths.begin(), fes_paths.end(), [&](const std::string& path) {
                    return path.find(filename_hint) != std::string::npos;
                });

                fragment.path = *it;
            }
        }
    }
}
