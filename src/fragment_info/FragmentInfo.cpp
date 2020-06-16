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

#include "FragmentInfo.hpp"

#include <boost/log/trivial.hpp>


#include <fmt/format.h>


namespace fesutils {

    FragmentRelativeGeometry FragmentRelativeGeometry_factory(FragmentRelativeGeometryType geom_type) {
        FragmentRelativeGeometry frgeom;

        if(geom_type == FragmentRelativeGeometryType::SingleCoord) {
            frgeom.type = FragmentRelativeGeometryType::SingleCoord;
            frgeom.internal_data =  std::make_shared<FragGeo_SingleCoord>();

        } else {
            BOOST_LOG_TRIVIAL(error) << "Unknown fragment relative geometry type. Cannot fabricate object of the appropriate object.";
            throw std::runtime_error("Unknown fragment relative geometry type");
        }

        return frgeom;
    }

    std::ostream& operator<<(std::ostream& os, const FragmentInfo& fi) {
        os << std::string("Fragment information\n");
        os << std::string("      Whole molecule name: ") + fi.name + std::string(" (") + fi.shortname + std::string(")\n");
        os << std::string("      Fragments: \n");
        for(const FragmentInfo::Fragment& fragment: fi.fragments) {
            os << std::string("        ") + fragment.name + std::string(" (") + fragment.shortname + std::string(")\n");
            os << std::string("          File hint: ") + fragment.filename_hint << "\n";
            os << std::string("          File path: ") + fragment.path << "\n";
            os << std::string("          Relative geometries (") << fragment.relativeGeometries.size() << "):\n";
            for(const auto & relativeGeometryMapPair : fragment.relativeGeometries) {
                os << std::string("            - ") <<relativeGeometryMapPair.first << " = " << relativeGeometryMapPair.second.internal_data->to_string() << "\n";
            }
        }

        return os;
    }

    FragGeo_SingleCoord::FragGeo_SingleCoord() :
            coord({0.0, 0.0, 0.0})
    {

    }

    std::string FragGeo_SingleCoord::to_string() {
        std::string ret = fmt::format("SingleCoord [{:6.3f} {:6.3f} {:6.3f}]", this->coord[0], this->coord[1], this->coord[2]);
        return ret;
    }

    FragGeo_SingleCoord::~FragGeo_SingleCoord() {

    }

    std::string FragmentRelativeGeometry_internalData::to_string() {
        return std::string("Base class FragmentRelativeGeometry_internalData");
    }

    std::ostream& operator<<(std::ostream& os, const FragmentRelativeGeometryType& frgtype) {
        if(frgtype == FragmentRelativeGeometryType::SingleCoord) {
            os << "single_coord";
        } else {
            os << "unknown_fragment_relative_geometry";
        }
        return os;
    }
}