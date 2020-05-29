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
#include "PlumedDatHeader.hpp"

#include <algorithm>

#include <boost/log/trivial.hpp>


namespace fesutils {

    // LCOV_EXCL_START
    // Reason for coverage exclusion: purely console printing function

    std::ostream& operator<<(std::ostream& os, const PlumedDatHeader& hd) {
        os << std::string("Fields: \n");

        for(int i = 0; i< hd.fields.size(); i++) {
            os << std::string("  [" + std::to_string(i) + "] " + hd.fields[i].name + "\n");
            os << std::string("     Attrs: ") +  "\n";
            for(auto it = hd.fields[i].attributes.begin(); it != hd.fields[i].attributes.end(); it++) {
                std::string val_str;
                std::visit([&val_str](auto&& arg){val_str = std::to_string(arg);}, it->second);
                os << (std::string("             [\"") + it->first + std::string("\"] = ") + val_str + "\n");
            }
        }

        return os;
    }

    size_t PlumedDatHeader::find_field_index_from_name(const std::string& name) {

        auto it = std::find_if(this->fields.begin(), this->fields.end(), [&](const Field& f) {
            return f.name == name; // Find field that has bias
        });
        if(it != this->fields.end()) {
            return std::distance(this->fields.begin(), it);
        } else {
            BOOST_LOG_TRIVIAL(error) << "Requesting index of field \"" << name << "\" which is not present in this header";
            if(this->originating_file_path.has_value()) {
                BOOST_LOG_TRIVIAL(error) << "  Header was parsed from: " << this->originating_file_path.value();
            }
            BOOST_LOG_TRIVIAL(error) << "  Existing field names: ";
            for(const auto& field: this->fields) {
                BOOST_LOG_TRIVIAL(error) << "     - " << field.name;
            }
            throw std::runtime_error("Requested field is not present in header");
        }
    }

    // LCOV_EXCL_STOP

}

