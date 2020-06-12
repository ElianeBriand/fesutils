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

#ifndef FESUTILS_READ_CV_FILE_HEADER_HPP
#define FESUTILS_READ_CV_FILE_HEADER_HPP

#include <string>

#include "PlumedDatHeader.hpp"

namespace fesutils {

    /**
     *
     * \param path Path to the CV file
     * \return filled PlumedDatHeader
     */
    PlumedDatHeader read_cv_file_header(const std::string& path);

    // //////////// Internals exposed for testing purposes  ////////////

    /** Extract fields from the corresponding header line
     * PRECONDITION: line starts with "#! FIELDS "
     * \param header PlumedDatHeader to fill
     * \param line line to extract fields from
     */
    void parse_FIELDS_line(PlumedDatHeader& header, const std::string& line);

    /** Extract information related to fields from the corresponding header line
     * PRECONDITION: line starts with "#! SET "
     * \param header PlumedDatHeader to fill
     * \param line line to extract fields from
     */
    void parse_SET_line(PlumedDatHeader& header, const std::string& line);


    /** Set global attribute in header given string representation of attribute and value
     * \param header PlumedDatHeader to fill
     * \param attribute_name attribute name string
     * \param value value string
     */
    void set_global_attribute(PlumedDatHeader& header, const std::string& attribute_name, const std::string& value);

    /** Set field-linked attribute in header given string representation of attribute and value
     * \param header PlumedDatHeader to fill
     * \param attribute_name attribute name string
     * \param field_name field name as string
     * \param value value string
     */
    void set_field_attribute(PlumedDatHeader& header, const std::string& attribute_name, const std::string& field_name, const std::string& value);


    /**
     * Extract the components of a "basename.subfield" field name. Does not support multiple levels (basename.subfield.subfield)
     * or empty subfield: in these cases, return whole field as basename.
     *
     * \param field
     * \return tuple of (basename, subfield)
     */
    std::tuple<std::string, std::string> extract_basename_subfield_from_field_name(const std::string& field);

    /**
     * Extract the attribute name, and the field name from a "attrribute_fieldname" attribute field.
     *
     * \param attrfield
     * \return tuple of (attribute name, field name)
     */
    std::tuple<std::string, std::string> extract_attribute_field_from_attrfield(const std::string& attrfield);

    /** Identify which fields are grid dimensions
    */
    std::vector<size_t> find_fields_index_that_have_required_attribute_for_grid(const PlumedDatHeader& header);

    /** Is this field possibly a grid dimension ? From attribute presence
    */
    bool is_field_a_grid_dimension(const Field& field);

    /** Remove fields that are not grid dimensions (lacking min/max/etc attributes)
    */
    void remove_nongrid_fields(PlumedDatHeader& header);

}


#endif //FESUTILS_READ_CV_FILE_HEADER_HPP
