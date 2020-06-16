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

#ifndef FESUTILS_FRAGMENTINFO_HPP
#define FESUTILS_FRAGMENTINFO_HPP

#include <string>
#include <vector>
#include <array>
#include <memory>
#include <map>

namespace fesutils {

    enum FragmentRelativeGeometryType {
        SingleCoord
    };

    struct FragmentRelativeGeometry_internalData {
        virtual ~FragmentRelativeGeometry_internalData() = default;
        virtual std::string to_string();
    };

    struct FragGeo_SingleCoord : public FragmentRelativeGeometry_internalData {
        FragGeo_SingleCoord();
        std::array<double, 3> coord;
        virtual ~FragGeo_SingleCoord();

        virtual std::string to_string();
    };

    struct FragmentRelativeGeometry {
        FragmentRelativeGeometryType type;

        std::shared_ptr<FragmentRelativeGeometry_internalData> internal_data;
    };



    FragmentRelativeGeometry FragmentRelativeGeometry_factory(FragmentRelativeGeometryType geom_type);

    struct FragmentInfo {

        friend std::ostream& operator<<(std::ostream& os, const FragmentInfo& fi);


        struct Fragment {
            std::string shortname;
            std::string name;
            std::string filename_hint; // filename as in the YAML file
            std::string path;          // actual file path

            std::map<FragmentRelativeGeometryType, FragmentRelativeGeometry> relativeGeometries;
        };

        std::string name;
        std::string shortname;

        std::vector<Fragment> fragments;
    };

    std::ostream& operator<<(std::ostream& os, const FragmentInfo& fi);

    std::ostream& operator<<(std::ostream& os, const FragmentRelativeGeometryType& frgtype);

}



#endif //FESUTILS_FRAGMENTINFO_HPP
