//
// Created by eliane on 20/03/2020.
//

#ifndef FESUTILS_FS_UTILS_HPP
#define FESUTILS_FS_UTILS_HPP

#include <string>

namespace fesutils {

    /**
     * Check if a path exist, and is a normal file
     * \param filepath path to check
     * \return true if the file exists and is a normal file
     */
    bool path_exists_isfile(const std::string& filepath);

    /** Return an error message for why the file could not be opened.
     *
     * PRECONDITION: path_exists_isfile called on filepath, returned false
     *
     * \param filepath path to check
     * \return a string containing the error message
     * \sa path_exists_isfile
     */
    std::string path_notfile_whatistheproblem(const std::string& filepath);

}

#endif //FESUTILS_FS_UTILS_HPP
