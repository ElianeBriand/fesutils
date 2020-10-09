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

#include "read_file_header.hpp"

#include <fstream>
#include <tuple>
#include <set>

#include <boost/log/trivial.hpp>
#include <boost/lexical_cast.hpp>


#include "../common/fs_utils.hpp"
#include "../common/extract_from_string.hpp"

namespace b = boost;

namespace fesutils {

    void parse_FIELDS_line(PlumedDatHeader& header, const std::string& line) {

        std::vector<std::string> tokens = slow_tokenize(line);

        if(tokens.size() < 3) {
            BOOST_LOG_TRIVIAL(error) << "FIELDS header line: " << line;
            BOOST_LOG_TRIVIAL(error) << "contains no field names";
            throw std::runtime_error("Incorrect FIELDS header line");
        }

        for(int i = 2; i < tokens.size(); i++) {
            Field f;
            auto  [basename, subfield] = extract_basename_subfield_from_field_name(tokens.at(i));
            f.name = tokens.at(i);

            if(subfield == "") {
                f.is_subcomponent = false;
                f.basename = basename;
                f.subfield = "";
            }else {
                f.is_subcomponent = true;
                f.basename = basename;
                f.subfield = subfield;
            }
            header.fields.push_back(std::move(f));

        }

    }

    void parse_SET_line(PlumedDatHeader& header, const std::string& line) {


        std::vector<std::string> tokens = slow_tokenize(line);

        if(tokens.size() < 4) {
            BOOST_LOG_TRIVIAL(error) << "SET header line: " << line;
            BOOST_LOG_TRIVIAL(error) << "is malformed. Typical #! SET <attribute>_<varname> <value>";
            throw std::runtime_error("Incorrect SET header line");
        }
        // FIXME: refactor into structured binding when CLion can understand them correctly
        auto res  = extract_attribute_field_from_attrfield(tokens.at(2));
        const std::string attribute_name = std::get<0>(res);
        const std::string fieldname = std::get<1>(res);

        const std::string& value = tokens.at(3);

        if(fieldname == "") {
            // Non-variable-linked attribute
            set_global_attribute(header, attribute_name,value);
        } else {
            // Variable-linked attribute
            set_field_attribute(header, attribute_name, fieldname, value);
        }
    }

    std::tuple<std::string, std::string> extract_basename_subfield_from_field_name(const std::string& field) {
        size_t pos_dot =  field.find('.');
        size_t pos_second_dot =  field.find('.', pos_dot+1);

        // If dot not found, or more than 1 dot, or no text after the dot
        if (pos_dot == std::string::npos ||
            pos_second_dot != std::string::npos ||
            pos_dot == field.size() - 1) {
            return std::make_tuple(field,"");
        }


        return std::make_tuple(field.substr(0,pos_dot),field.substr(pos_dot+1));
    }

    std::tuple<std::string, std::string> extract_attribute_field_from_attrfield(const std::string& attrfield) {
        size_t first_underscore =  attrfield.find('_');

        if (first_underscore == std::string::npos) {
            // No "_" in attrfield -> no a variable-related attribute
            return std::make_tuple(attrfield, "");
        }

        std::tuple<std::string, std::string> result = std::make_tuple(attrfield.substr(0,first_underscore), attrfield.substr(first_underscore+1));




        if(std::get<0>(result) == "") {
            BOOST_LOG_TRIVIAL(error) << "Attribute field :" << attrfield;
            BOOST_LOG_TRIVIAL(error) << "has empty attribute name. Typical <attribute>_<varname>";
            throw std::runtime_error("Incorrect attribute field formatting");
        }

        if(std::get<1>(result) == "") {
            BOOST_LOG_TRIVIAL(error) << "Attribute field :" << attrfield;
            BOOST_LOG_TRIVIAL(error) << "has empty varname. Typical <attribute>_<varname>";
            throw std::runtime_error("Incorrect attribute field formatting");
        }

        return result;

    }



    PlumedDatHeader read_plumed_file_header(const std::string& path) {
        PlumedDatHeader header;

        if(!path_exists_isfile(path)) {
            BOOST_LOG_TRIVIAL(error) << "Path " << path << "cannot be opened: "  << path_notfile_whatistheproblem(path);
            throw std::runtime_error("Cannot open CV file");
        }

        std::ifstream inputf(path);

        header.originating_file_path = path;

        std::string line;
        while(getline( inputf, line ) ) {
            if (line.empty() || line[0] != '#') {
                break;
            }

            if (line.find("#! FIELDS ") != std::string::npos) {
                parse_FIELDS_line(header, line);
            } else if(line.find("#! SET ") != std::string::npos) {
                parse_SET_line(header, line);
            } else {
                BOOST_LOG_TRIVIAL(debug) << "Header line was not used: " << line;
            }
        }


        return header;
    }


    void set_global_attribute(PlumedDatHeader& header, const std::string& attribute_name, const std::string& value) {
        static const std::set<std::string> attribute_double = {"normalisation"};
        static const std::set<std::string> attribute_int = {};
        static const std::set<std::string> attribute_bool = {};

        if(attribute_double.count(attribute_name)) {
            double attr_value = 0.0;
            try {
                attr_value = boost::lexical_cast<double>(value);
            } catch (boost::bad_lexical_cast& e) {
                BOOST_LOG_TRIVIAL(error) << "Attribute " << attribute_name << " has value that should be parseable to double,"
                                                                              " but is not. ( value = \"" << value << "\")";
                throw std::runtime_error("Incorrect SET attribute value");
            }
            header.attributes[attribute_name] = attr_value;


        /*   // Currently only double-valued global attribute exists
        } else if(attribute_bool.count(attribute_name)) {
            bool attr_value = false;
            try {
                attr_value = parse_bool_as_word(value);
            } catch (std::runtime_error& e) {
                BOOST_LOG_TRIVIAL(error) << "Attribute " << attribute_name << " has value that should be parseable to bool,"
                                                                              " but is not. ( value = \"" << value << "\")";

                throw std::runtime_error("Incorrect SET attribute value");
            }
            header.attributes[attribute_name] = attr_value;
        } else if(attribute_int.count(attribute_name)) {
            int attr_value = 0;
            try {
                attr_value = boost::lexical_cast<int>(value);
            } catch (boost::bad_lexical_cast& e) {
                BOOST_LOG_TRIVIAL(error) << "Attribute " << attribute_name << " has value that should be parseable to int,"
                                                                              " but is not. ( value = \"" << value << "\")";
                throw std::runtime_error("Incorrect SET attribute value");
            }
            header.attributes[attribute_name] = attr_value;
            */
        } else {
            BOOST_LOG_TRIVIAL(warning) << "Unhandled SET attribute" << attribute_name << " has been discarded.";
        }

    }





    void set_field_attribute(PlumedDatHeader& header, const std::string& attribute_name, const std::string& field_name, const std::string& value) {
        static const std::set<std::string> attribute_double = {"min", "max"};
        static const std::set<std::string> attribute_int = {"nbins"};
        static const std::set<std::string> attribute_bool = {"periodic"};

        auto field_it = std::find_if(header.fields.begin(), header.fields.end(), [&field_name](const Field& elem) {
            return (elem.name) == field_name;
        });

        if(attribute_double.count(attribute_name)) {
            double attr_value = 0.0;
            try {
                attr_value = boost::lexical_cast<double>(value);
            } catch (boost::bad_lexical_cast& e) {
                BOOST_LOG_TRIVIAL(error) << "Attribute " << attribute_name << " has value that should be parseable to double,"
                                                                              " but is not. ( value = \"" << value << "\")";
                throw std::runtime_error("Incorrect SET attribute value");
            }
            field_it->attributes[attribute_name] = attr_value;

        } else if(attribute_bool.count(attribute_name)) {
            bool attr_value = false;
            try {
                attr_value = parse_bool_as_word(value);
            } catch (std::runtime_error& e) {
                BOOST_LOG_TRIVIAL(error) << "Attribute " << attribute_name << " has value that should be parseable to bool,"
                                                                              " but is not. ( value = \"" << value << "\")";

                throw std::runtime_error("Incorrect SET attribute value");
            }
            field_it->attributes[attribute_name] = attr_value;
        } else if(attribute_int.count(attribute_name)) {
            int attr_value = 0;
            try {
                attr_value = boost::lexical_cast<int>(value);
            } catch (boost::bad_lexical_cast& e) {
                BOOST_LOG_TRIVIAL(error) << "Attribute " << attribute_name << " has value that should be parseable to int,"
                                                                              " but is not. ( value = \"" << value << "\")";
                throw std::runtime_error("Incorrect SET attribute value");
            }
            field_it->attributes[attribute_name] = attr_value;
        } else {
            BOOST_LOG_TRIVIAL(warning) << "Unhandled SET attribute" << attribute_name << " has been discarded.";
        }
    }


    void remove_nongrid_fields(PlumedDatHeader& header) {
        // Loop until no more nongrid field are found
        // (finding then removing list of not necessarily continous indices from vector is a bit tricky)

        bool hasFoundNonGridField;
        do {
            hasFoundNonGridField = false;
            for (int i = 0; i < header.fields.size(); i++) {
                const Field field = header.fields[i];
                if (!is_field_a_grid_dimension(field)) {
                    // Missing attribute
                    header.fields.erase(header.fields.begin() + i);
                    hasFoundNonGridField = true;
                    break; // Index i is ""invalidated"", better restart
                }
            }
        } while(hasFoundNonGridField);
    }

    bool is_field_a_grid_dimension(const Field& field) {
        if (!field.attributes.count("min") ||
            !field.attributes.count("max") ||
            !field.attributes.count("nbins") ||
            !field.attributes.count("periodic")) {
            // Missing attribute
            return false;
        } else {
            return true;
        }
    }

}