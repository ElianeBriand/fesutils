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


#include <boost/log/trivial.hpp>


namespace fesutils {

    // LCOV_EXCL_START
    // Reason for coverage exclusion: subject to system testing, not unit testing
    // System test script: st_reconstruct_from_fragments.py

    int do_reconstruct_from_fragments(GeneralOptions& options,
                                       const reconstruct_from_fragments_args& args) {
/*

        BOOST_LOG_TRIVIAL(info) << "Fragment FES files             : ";
        for (const std::string& grid_path: args.fragment_fes_paths) {
            BOOST_LOG_TRIVIAL(info) << "                                 - " << grid_path;
        }

        BOOST_LOG_TRIVIAL(info) << "Output file                    : " << args.outfile_path;
        BOOST_LOG_TRIVIAL(info) << "Reconstruction algorithm       : " << args.algorithm;
        BOOST_LOG_TRIVIAL(info) << "Fragment information YAML      : " << args.fragment_info_yaml_path;

        BOOST_LOG_TRIVIAL(info) << "";
        BOOST_LOG_TRIVIAL(info) << "[Parsing fragment info]";

        FragmentInfo fragmentInfo = parse_fragment_info_yaml(args.fragment_info_yaml_path, args.fragment_fes_paths);
        size_t num_fragments = fragmentInfo.fragments.size();
        BOOST_LOG_TRIVIAL(info) << "Total number of fragments: " << num_fragments;
        BOOST_LOG_TRIVIAL(info) << "";
        BOOST_LOG_TRIVIAL(info) << "[Loading FES data]";

        std::vector<std::shared_ptr<Grid>> fes_grids;
        std::vector<PlumedDatHeader> fes_grid_headers;

        for (auto& fragment : fragmentInfo.fragments) {
            const std::string fes_path = fragment.path;
            PlumedDatHeader header_grid = read_plumed_file_header(fes_path);
            std::shared_ptr<Grid> p = ingest_fes_grid(options, fes_path, header_grid, args.fefield);
            fes_grid_headers.push_back(header_grid);
            fes_grids.push_back(p);
        }

        BOOST_LOG_TRIVIAL(info) << "";
        BOOST_LOG_TRIVIAL(info) << "[Reconstructing]";

        BOOST_LOG_TRIVIAL(info) << "Using algorithm: " << args.algorithm;
        std::shared_ptr<Grid> result_grid;
        if (args.algorithm == "verynaive") {
            result_grid = reconstruction_verynaive_algorithm(fes_grids, fes_grid_headers,
                                                                                   fragmentInfo);
        } else if (args.algorithm == "verynaive_withdisplacement") {
            result_grid = reconstruction_verynaive_withdisplacement_algorithm(fes_grids, fes_grid_headers,
                                                                              fragmentInfo);
        }else if (args.algorithm == "verynaive_conformerrotation") {
            result_grid = reconstruction_verynaive_conformerrotation_algorithm(fes_grids, fes_grid_headers,
                                                                               fragmentInfo);
        }else if (args.algorithm == "verynaive_multiconfrotation") {
            result_grid = reconstruction_verynaive_multiconfrotation_algorithm(fes_grids, fes_grid_headers,
                                                                               fragmentInfo);
        }else {
            BOOST_LOG_TRIVIAL(info) << "Unknown algorithm: " << args.algorithm;
            throw std::runtime_error("Unknown algorithm");
        }


        BOOST_LOG_TRIVIAL(info) << "";
        BOOST_LOG_TRIVIAL(info) << "[Writing output]";

        write_grid_to_opendx(*result_grid, args.outfile_path);

        BOOST_LOG_TRIVIAL(info) << "Done.";
        */
        return 0;
    }

    // LCOV_EXCL_STOP

}