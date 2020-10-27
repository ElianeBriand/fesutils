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

#ifndef FESUTILS_PLUMEDDATHEADER_HPP
#define FESUTILS_PLUMEDDATHEADER_HPP

#include <vector>
#include <string>
#include <map>
#include <variant>

namespace fesutils {

    /**
     * Hold data related to a Plumed .dat file header field
     */
    struct Field {
        std::string name;

        bool is_subcomponent = false;
        std::string basename;
        std::string subfield;

        std::map<std::string, std::variant<bool, int, double>> attributes;

        Field() = default;
    };

    /**
     * Holds data extracted from a Plumed-generated .dat file.
     */


    struct PlumedDatHeader {
        std::optional<std::string> originating_file_path;

        std::vector<Field> fields;


        friend std::ostream& operator<<(std::ostream& os, const PlumedDatHeader& hd);

        [[nodiscard]] size_t find_field_index_from_name(const std::string& name) const;

        std::map<std::string, std::variant<bool, int, double>> attributes;


    };

    std::ostream& operator<<(std::ostream& os, const PlumedDatHeader& hd);

}




#endif //FESUTILS_PLUMEDDATHEADER_HPP
