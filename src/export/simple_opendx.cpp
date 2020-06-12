//
// Created by eliane on 12/06/2020.
//

#include <fstream>
#include <fmt/format.h>
#include <boost/log/trivial.hpp>


#include "simple_opendx.hpp"
#include "../grid/grid_utils.hpp"

namespace fesutils {

    namespace {
        std::string dims_number_string(const fesutils::Grid& grid) {
            std::string ret;
            for(int i = 0; i < grid.num_dims; i++) {
                ret += fmt::format("{:d} ", grid.dims[i]);
            }
            ret.pop_back();
            return ret;
        }

        std::string origin_string_angstrom(const fesutils::Grid& grid) {
            std::string ret;
            for(int i = 0; i < grid.num_dims; i++) {
                ret += fmt::format("{:8.8f} ", grid.min_vals[i] * 10.0);
            }
            ret.pop_back();
            return ret;
        }

        std::string delta_strings_angstrom(const fesutils::Grid& grid) {
            std::string ret;
            for(int i = 0; i< grid.num_dims; i++) {
                std::vector<double> unit_vect(grid.num_dims, 0.0);
                unit_vect[i] = grid.bin_sizes[i];
                ret += "delta  ";
                for(int j = 0; j < grid.num_dims; j++) {
                    ret += fmt::format("{:8.8f} ", unit_vect[j] * 10.0);
                }
                ret.pop_back();
                ret += "\n";
            }
            return ret;
        }

        int num_datapoint(const fesutils::Grid& grid) {
            int num_datapoint = 1;
            for(int i = 0; i< grid.num_dims; i++) {
                num_datapoint *= grid.dims[i];
            }
            return num_datapoint;
        }
    }




    void write_grid_to_opendx(const fesutils::Grid& grid, const std::string& filename) {

        BOOST_LOG_TRIVIAL(info) << "Writing OpenDx file " << filename;

        std::ofstream dxfile;
        dxfile.open (filename);
        dxfile << "# OpenDX written by fesutils simple_opendx.hpp\n";
        dxfile << "object 1 class gridpositions counts  "  << dims_number_string(grid) << "\n";
        dxfile << "origin " <<  origin_string_angstrom(grid) <<" \n";
        dxfile << delta_strings_angstrom(grid);
        dxfile << "object 2 class gridconnections counts  " << dims_number_string(grid) << "\n";
        dxfile << fmt::format("object 3 class array type \"double\" rank 0 items {:d} data follows\n", num_datapoint(grid) );

        double value = 0.0;
        std::vector<long> indexes(grid.num_dims, 0);
        int currDim = 0;
        while(true) {
            grid.grid_array->get(indexes, value);

            dxfile << fmt::format("{:18.18f}", value);

            currDim += 1;
            if(currDim < grid.num_dims) {
                dxfile << " ";
            } else {
                dxfile << "\n";
                currDim = 0;
            }

            // Increment indexes until reaching the end
            if(!increment_grid_indexes_last_first(grid, indexes)) {
                break;
            }
        }
        dxfile << "\n";
        dxfile << "attribute \"dep\" string \"positions\"\n"
                  "object \"density\" class field \n"
                  "component \"positions\" value 1\n"
                  "component \"connections\" value 2\n"
                  "component \"data\" value 3\n";



        dxfile.close();

        BOOST_LOG_TRIVIAL(info) << "Done writing file.";

    }
}

