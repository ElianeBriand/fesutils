//
// Created by eliane on 20/03/2020.
//

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

    /**
     * Extract the components of a "basename.subfield" field name. Does not support multiple levels (basename.subfield.subfield)
     * or empty subfield: in these cases, return whole field as basename.
     *
     * \param field
     * \return tuple of (basename, subfield)
     */
    std::tuple<std::string, std::string> extract_basename_subfield_from_field_name(const std::string& field);
}


#endif //FESUTILS_READ_CV_FILE_HEADER_HPP
