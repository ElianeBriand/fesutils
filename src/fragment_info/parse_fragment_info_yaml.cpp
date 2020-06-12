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

#include "parse_fragment_info_yaml.hpp"
#include "FragmentInfo_Utils.hpp"

#include <boost/log/trivial.hpp>
#include <boost/algorithm/string.hpp>

#include <yaml-cpp/yaml.h>



namespace fesutils {

    void parse_fragment_geometry_record(YAML::const_iterator& iter, FragmentInfo::Fragment& curr_fragment) ;

    FragmentInfo parse_fragment_info_yaml(const std::string& fragmentinfo_yaml_path, const std::vector<std::string>& fes_paths) {
        FragmentInfo fragmentInfo;

        YAML::Node fi_yaml = YAML::LoadFile(fragmentinfo_yaml_path);

        if (fi_yaml["whole_molecule"]) {
            if(fi_yaml["whole_molecule"]["name"]) {
                fragmentInfo.name = fi_yaml["whole_molecule"]["name"].as<std::string>();
            } else {
                fragmentInfo.name = "Ligand";
            }
            if(fi_yaml["whole_molecule"]["shortname"]) {
                fragmentInfo.shortname = fi_yaml["whole_molecule"]["shortname"].as<std::string>();
            } else {
                fragmentInfo.shortname = boost::to_upper_copy<std::string>(fragmentInfo.name.substr(0,3));
            }
        } else {
            BOOST_LOG_TRIVIAL(error) << "Missing mandatory section in fragment info YAML: whole_molecule";
            throw std::runtime_error("Missing mandatory fragment info section");
        }


        if (fi_yaml["fragments"]) {
            for (YAML::const_iterator it = fi_yaml["fragments"].begin(); it != fi_yaml["fragments"].end(); ++it) {
                YAML::Node  fragment_node = *it;
                if(fragment_node.size() != 1) {
                    BOOST_LOG_TRIVIAL(error) << "Misformated fragment node: should contain a top-level dict with a shorthand name as key";
                    throw std::runtime_error("Misformated fragment node");
                }
                FragmentInfo::Fragment fragment;

                fragment.shortname = it->begin()->first.as<std::string>();

                YAML::Node  fragmentinfo_dict_node = it->begin()->second;
                if(fragmentinfo_dict_node["name"]) {
                    fragment.name = fragmentinfo_dict_node["name"].as<std::string>();
                }else {
                    BOOST_LOG_TRIVIAL(error) << "Missing name for fragment in fragment info.";
                    throw std::runtime_error("Missing name for fragment in fragment info.");
                }

                if(fragmentinfo_dict_node["filename_hint"]) {
                    fragment.filename_hint = fragmentinfo_dict_node["filename_hint"].as<std::string>();
                }else {
                    BOOST_LOG_TRIVIAL(error) << "Missing filename_hint for fragment in fragment info.";
                    throw std::runtime_error("Missing filename_hint for fragment in fragment info.");
                }

                if(fragmentinfo_dict_node["geometry"]) {
                    // Do geometry
                    for (YAML::const_iterator it_geom = fragmentinfo_dict_node["geometry"].begin(); it_geom != fragmentinfo_dict_node["geometry"].end(); ++it_geom) {
                        parse_fragment_geometry_record(it_geom, fragment);
                    }
                }else {
                    BOOST_LOG_TRIVIAL(error) << "Missing geometry data for fragment in fragment info.";
                    throw std::runtime_error("Missing geometry for fragment in fragment info.");
                }

                fragmentInfo.fragments.push_back(std::move(fragment));
            }
        } else {
            BOOST_LOG_TRIVIAL(error) << "Missing mandatory section in fragment info YAML: fragments";
            throw std::runtime_error("Missing mandatory fragment info section");
        }

        fragment_path_from_filename(fragmentInfo, fes_paths);


        BOOST_LOG_TRIVIAL(info) << fragmentInfo;


        return fragmentInfo;
    }


    void parse_fragment_geometry_record(YAML::const_iterator& iter, FragmentInfo::Fragment& curr_fragment) {
        std::string geom_type = iter->first.as<std::string>();

        if(geom_type == "single_coord") {
            if(iter->second.Type() == YAML::NodeType::Sequence && iter->second.size() == 3) {
                FragmentRelativeGeometry frg =  FragmentRelativeGeometry_factory(FragmentRelativeGeometryType::SingleCoord);
                FragGeo_SingleCoord* data = dynamic_cast<FragGeo_SingleCoord*>(frg.internal_data.get());
                assert(data != nullptr);
                try  {
                    data->coord = { iter->second[0].as<double>() ,iter->second[1].as<double>(), iter->second[2].as<double>() };
                }
                catch(...) {
                    BOOST_LOG_TRIVIAL(error) << "Could not read coordinate vector: \"" << iter->second << "\"";
                    return;
                }
                curr_fragment.relativeGeometries.push_back(frg);
            } else {
                BOOST_LOG_TRIVIAL(error) << "Fragment geometry type single_coord require a 3D position vector argument (like \"[2.4, 12.0, -1.2]\")";
                return;
            }
        } else {
            BOOST_LOG_TRIVIAL(error) << "Unknown geometry type \"" << geom_type << "\" in fragment information, ignored.";
            return;
        }

    }

}