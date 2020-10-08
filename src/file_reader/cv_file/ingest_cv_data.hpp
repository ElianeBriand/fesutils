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

#ifndef FESUTILS_INGEST_CV_DATA_HPP
#define FESUTILS_INGEST_CV_DATA_HPP

#include <boost/filesystem.hpp>
namespace bf = boost::filesystem;

#include "../../file_reader/PlumedDatHeader.hpp"
#include "../../GeneralOptions.h"
#include "CVData.hpp"

namespace fesutils {
    bool ingest_cv_data(const GeneralOptions& options,
                        const bf::path& cv_file_path,
                        std::shared_ptr<CVData> cv_data_container,
                        const PlumedDatHeader& cv_file_header,
                        const std::vector<std::string>& cv_varnames,
                        const std::string& metad_bias_varname,
                        const std::vector<std::string>& other_values_varnames);

    bool ingest_cv_data_from_bindump(const GeneralOptions& options,
                        const bf::path& cv_file_path,
                        std::shared_ptr<CVData> cv_data_container,
                        const PlumedDatHeader& cv_file_header,
                        const std::vector<std::string>& cv_varnames,
                        const std::string& metad_bias_varname,
                        const std::vector<std::string>& other_values_varnames,
                        const std::vector<size_t>& cv_vars_idx,
                        const size_t& metad_bias_var_idx,
                        const std::vector<size_t>& other_vars_idx,
                        const bf::path& bindump_path);

    bool ingest_cv_data_from_originalfile(const GeneralOptions& options,
                                     const bf::path& cv_file_path,
                                     std::shared_ptr<CVData> cv_data_container,
                                     const PlumedDatHeader& cv_file_header,
                                     const std::vector<std::string>& cv_varnames,
                                     const std::string& metad_bias_varname,
                                     const std::vector<std::string>& other_values_varnames,
                                     const std::vector<size_t>& cv_vars_idx,
                                     const size_t& metad_bias_var_idx,
                                     const std::vector<size_t>& other_vars_idx);

}


#endif //FESUTILS_INGEST_CV_DATA_HPP
