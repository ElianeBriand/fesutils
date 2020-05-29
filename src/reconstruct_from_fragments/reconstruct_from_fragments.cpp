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

#include "reconstruct_from_fragments.hpp"
#include "../grid/Grid.hpp"
#include "../file_reader/read_file_header.hpp"
#include "../grid/ingest_grid.hpp"

#include <boost/log/trivial.hpp>


namespace fesutils {

    // LCOV_EXCL_START
    // Reason for coverage exclusion: subject to system testing, not unit testing
    // System test script: st_reconstruct_from_fragments.py

    int do_reconstruct_from_fragments(GeneralOptions& options,
                                       const reconstruct_from_fragments_args& args) {

        BOOST_LOG_TRIVIAL(info) << "Loading FES files: ";

        std::vector<std::shared_ptr<Grid>> fes_grids;
        std::vector<PlumedDatHeader> fes_grid_headers;
        for(const std::string& grid_path: args.fragment_fes_paths) {
            BOOST_LOG_TRIVIAL(info) << "   -- " << grid_path << " -- ";
            PlumedDatHeader header_grid =  read_cv_file_header(grid_path);
            std::shared_ptr<Grid> p =  ingest_fes_grid(options, grid_path, header_grid, args.fefield);
            fes_grid_headers.push_back(std::move(header_grid));
            fes_grids.push_back(std::move(p));
        }

        BOOST_LOG_TRIVIAL(info) << "Done.";
        return 0;
    }

    // LCOV_EXCL_STOP

}