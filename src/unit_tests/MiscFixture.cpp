//
// Created by eliane on 06/04/2020.
//

#include "MiscFixture.hpp"

MiscFixture::MiscFixture() {

    expected_num_grid_fields = 2;

    f::Field field1;
    field1.name = "varname1";
    field1.attributes["min"] = 1.0;
    expected_grid_min_vals.push_back(1.0);
    field1.attributes["max"] = 5.0;
    expected_grid_max_vals.push_back(5.0);
    field1.attributes["nbins"] = 4;
    expected_grid_dims.push_back(4);
    expected_grid_bin_sizes.push_back(1.0);
    field1.attributes["periodic"] = false;
    std::vector<double> field1_binedges = {1.0, 2.0, 3.0, 4.0, 5.0};
    expected_grid_bin_edges.push_back(std::move(field1_binedges));
    std::vector<double> field1_centers = {1.5, 2.5, 3.5, 4.5};
    expected_grid_bin_centers.push_back(std::move(field1_centers));

    f::Field field2;
    field2.name = "varname2";
    field2.attributes["min"] = -10.0;
    expected_grid_min_vals.push_back(-10.0);
    field2.attributes["max"] = +10.0;
    expected_grid_max_vals.push_back(10.0);
    field2.attributes["nbins"] = 10;
    expected_grid_dims.push_back(10);
    expected_grid_bin_sizes.push_back(2.0);
    field2.attributes["periodic"] = false;
    std::vector<double> field2_binedges = {-10.0, -8.0, -6.0,-4.0,-2.0,0.0,2.0,4.0,6.0,8.0,10.0};
    expected_grid_bin_edges.push_back(std::move(field2_binedges));
    std::vector<double> field2_centers = {-9.0, -7.0, -5.0, -3.0, -1.0, 1.0, 3.0, 5.0, 7.0, 9.0};
    expected_grid_bin_centers.push_back(std::move(field2_centers));

    f::Field field3;
    field3.name = "varname3.bias";

    this->dummy_header.fields.push_back(field1);
    this->dummy_header.fields.push_back(field2);
    this->dummy_header.fields.push_back(field3);
    this->index_bias_field = 2;

    expected_grid_field_indexes.push_back(0);
    expected_grid_field_indexes.push_back(1);

    std::vector<double> coord_vect(2);
    coord_vect = {1.2,  -2.5};
    example_coordinates_inside_grid.push_back(coord_vect);
    examples_value.push_back(42.3);

    coord_vect = {4.3,  8.9};
    example_coordinates_inside_grid.push_back(coord_vect);
    examples_value.push_back(-14.2);

    coord_vect = {3.7,  3.7};
    example_coordinates_inside_grid.push_back(coord_vect);
    examples_value.push_back(0.1);

    coord_vect = {-8.3,  12.4};
    example_coordinates_outside_grid.push_back(coord_vect);

    coord_vect = {3.7,  12.4};
    example_coordinates_outside_grid.push_back(coord_vect);

    coord_vect = {-8.7,  2.4};
    example_coordinates_outside_grid.push_back(coord_vect);
}
