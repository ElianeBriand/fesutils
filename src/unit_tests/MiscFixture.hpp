//
// Created by eliane on 06/04/2020.
//

#ifndef FESUTILS_MISCFIXTURE_HPP
#define FESUTILS_MISCFIXTURE_HPP

#include <vector>

#include "../file_reader/PlumedDatHeader.hpp"


namespace f = fesutils;

struct MiscFixture {

    MiscFixture();

    f::PlumedDatHeader dummy_header;

    size_t expected_num_grid_fields;
    size_t index_bias_field;
    std::vector<size_t> expected_grid_field_indexes;
    std::vector<size_t> expected_grid_dims;
    std::vector<double> expected_grid_min_vals;
    std::vector<double> expected_grid_max_vals;
    std::vector<double> expected_grid_bin_sizes;
    std::vector<std::vector<double>> expected_grid_bin_edges;
    std::vector<std::vector<double>> expected_grid_bin_centers;

    std::vector<std::vector<double>> example_coordinates_inside_grid;
    std::vector<double> examples_value;

    std::vector<std::vector<double>> example_coordinates_outside_grid;


};


#endif //FESUTILS_MISCFIXTURE_HPP
