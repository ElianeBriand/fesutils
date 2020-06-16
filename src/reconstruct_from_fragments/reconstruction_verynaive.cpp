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

#include "reconstruction_verynaive.hpp"
#include "reconstruction_utils.hpp"
#include "../grid/grid_utils.hpp"

#include <boost/log/trivial.hpp>

#include <Eigen/Dense>
#include <iostream>


// TODO: clean up the duplication in this file

namespace fesutils {

    std::shared_ptr<Grid> reconstruction_verynaive_algorithm(const std::vector<std::shared_ptr<Grid>>& fes_grids,
                                                             const std::vector<PlumedDatHeader>& fes_grid_headers,
                                                             const FragmentInfo& fragmentInfo) {

        BOOST_LOG_TRIVIAL(info) << "Creating result grid";

        // Make header with the minimal dimensions
        PlumedDatHeader result_header = grid_from_intersection_of_grids(fragmentInfo, fes_grid_headers);

        std::shared_ptr<Grid> result_grid = std::make_shared<Grid>(result_header);




        BOOST_LOG_TRIVIAL(info) << "Populating grid";
        std::vector<double> coord_vector((size_t) result_grid->num_dims);
        std::vector<size_t> idx_in_bin_centers((size_t) result_grid->num_dims, 0);
        while(true) {

            // Populate grid
            for(int curr_dim = 0; curr_dim < result_grid->num_dims; curr_dim++) {
                coord_vector[curr_dim] = result_grid->bin_center.at(curr_dim).at(idx_in_bin_centers.at(curr_dim));
            }
            if(!result_grid->check_inrange(coord_vector)) {
                BOOST_LOG_TRIVIAL(error) << "result_grid->check_inrange failed for generated coord_vector = " << coord_vector;
                throw std::runtime_error("Coordinate vector not in range");
            }

            double fes_value = 0.0;
            for(const auto & fes_grid : fes_grids) {
                if(!fes_grid->check_inrange(coord_vector)) {
                    BOOST_LOG_TRIVIAL(error) << "fes_grid->check_inrange failed for generated coord_vector = " << coord_vector;
                    throw std::runtime_error("Coordinate vector not in range");
                }
                fes_value += fes_grid->get_inrange(coord_vector);
            }
            result_grid->set_inrange(coord_vector, fes_value);

            // Increment indexes until reaching the end
            if(!increment_grid_indexes(*result_grid, idx_in_bin_centers )) {
                break;
            }
        }
        BOOST_LOG_TRIVIAL(info) << "Done populating grid";


        return result_grid;
    }


    std::shared_ptr<Grid> reconstruction_verynaive_withdisplacement_algorithm(const std::vector<std::shared_ptr<Grid>>& fes_grids,
                                                                              const std::vector<PlumedDatHeader>& fes_grid_headers,
                                                                              const FragmentInfo& fragmentInfo){
        BOOST_LOG_TRIVIAL(info) << "Creating result grid";

        // Make header with the minimal dimensions
        PlumedDatHeader result_header = grid_from_intersection_of_grids(fragmentInfo, fes_grid_headers);

        std::shared_ptr<Grid> result_grid = std::make_shared<Grid>(result_header);

        if(result_grid->num_dims != 3) {
            BOOST_LOG_TRIVIAL(info) << "Algorithm verynaive_withdisplacement works with exactly three spatial coordinate";
            throw std::runtime_error("Incorrect number of dimension for algorithm");
        }


        BOOST_LOG_TRIVIAL(info) << "Populating grid";
        std::vector<double> coord_vector((size_t) result_grid->num_dims);
        std::vector<double> orig_coord_vector((size_t) result_grid->num_dims);
        std::vector<size_t> idx_in_bin_centers((size_t) result_grid->num_dims, 0);
        while(true) {

            // Populate grid
            for(int curr_dim = 0; curr_dim < result_grid->num_dims; curr_dim++) {
                orig_coord_vector[curr_dim] = result_grid->bin_center.at(curr_dim).at(idx_in_bin_centers.at(curr_dim));
            }
            coord_vector = orig_coord_vector;

            if(!result_grid->check_inrange(coord_vector)) {
                BOOST_LOG_TRIVIAL(error) << "result_grid->check_inrange failed for generated coord_vector = " << coord_vector;
                throw std::runtime_error("Coordinate vector not in range");
            }

            double fes_value = 0.0;
            for(int i = 0; i < fragmentInfo.fragments.size(); i++) {
                coord_vector = orig_coord_vector;
                const PlumedDatHeader& curr_header = fes_grid_headers.at(i);
                const std::shared_ptr<Grid>& curr_grid = fes_grids.at(i);
                const FragmentInfo::Fragment& curr_fragment = fragmentInfo.fragments.at(i);

                FragGeo_SingleCoord* curr_singlecoorddata = dynamic_cast<FragGeo_SingleCoord*>(
                        curr_fragment.relativeGeometries.at(FragmentRelativeGeometryType::SingleCoord).internal_data.get()
                        );


                std::array<double, 3>& curr_coord = curr_singlecoorddata->coord;

                for(int j = 0; j < 3; j++){
                    coord_vector[j] += (curr_coord[j]/10.0);
                }

                if(curr_grid->check_inrange(coord_vector)) {
                    fes_value += curr_grid->get_inrange(coord_vector);
                }
            }
            result_grid->set_inrange(orig_coord_vector, fes_value);

            // Increment indexes until reaching the end
            if(!increment_grid_indexes(*result_grid, idx_in_bin_centers )) {
                break;
            }
        }
        BOOST_LOG_TRIVIAL(info) << "Done populating grid";


        return result_grid;


    }

    std::shared_ptr<Grid> reconstruction_verynaive_conformerrotation_algorithm(const std::vector<std::shared_ptr<Grid>>& fes_grids,
                                                                               const std::vector<PlumedDatHeader>& fes_grid_headers,
                                                                               const FragmentInfo& fragmentInfo){
        BOOST_LOG_TRIVIAL(info) << "Creating result grid";

        // Make header with the minimal dimensions
        PlumedDatHeader result_header = grid_from_intersection_of_grids(fragmentInfo, fes_grid_headers);

        std::shared_ptr<Grid> result_grid = std::make_shared<Grid>(result_header);

        if(result_grid->num_dims != 3) {
            BOOST_LOG_TRIVIAL(info) << "Algorithm verynaive_conformerrotation works with exactly three spatial coordinate";
            throw std::runtime_error("Incorrect number of dimension for algorithm");
        }


        BOOST_LOG_TRIVIAL(info) << "Populating grid";
        std::vector<double> coord_vector((size_t) result_grid->num_dims);
        std::vector<double> orig_coord_vector((size_t) result_grid->num_dims);
        std::vector<size_t> idx_in_bin_centers((size_t) result_grid->num_dims, 0);
        while(true) {

            // Populate grid
            for(int curr_dim = 0; curr_dim < result_grid->num_dims; curr_dim++) {
                orig_coord_vector[curr_dim] = result_grid->bin_center.at(curr_dim).at(idx_in_bin_centers.at(curr_dim));
            }
            coord_vector = orig_coord_vector;

            if(!result_grid->check_inrange(coord_vector)) {
                BOOST_LOG_TRIVIAL(error) << "result_grid->check_inrange failed for generated coord_vector = " << coord_vector;
                throw std::runtime_error("Coordinate vector not in range");
            }

            double fes_value = 0.0;
            const int num_subdiv_angle = 10;
            for(int x_step = 0; x_step < num_subdiv_angle; x_step ++) {
                const double curr_angle_x_radian = (2.0 * 3.14 / double(num_subdiv_angle)) * x_step;
                Eigen::AngleAxis<double> x_rotate(curr_angle_x_radian,
                                                  Eigen::Vector3d(1.0, 0.0, 0.0));
                for(int y_step = 0; x_step < num_subdiv_angle; x_step ++) {
                    const double curr_angle_y_radian = (2.0 * 3.14 / double(num_subdiv_angle)) * y_step;
                    Eigen::AngleAxis<double> y_rotate(curr_angle_y_radian,
                                                      Eigen::Vector3d(0.0, 1.0, 0.0));
                    for(int z_step = 0; x_step < num_subdiv_angle; x_step ++) {
                        const double curr_angle_z_radian = (2.0 * 3.14 / double(num_subdiv_angle)) * z_step;
                        Eigen::AngleAxis<double> z_rotate(curr_angle_z_radian,
                                                          Eigen::Vector3d(0.0, 0.0, 1.0));
                        double candidate_fes_value = 0.0;
                        for (int i = 0; i < fragmentInfo.fragments.size(); i++) {
                            coord_vector = orig_coord_vector;
                            const PlumedDatHeader& curr_header = fes_grid_headers.at(i);
                            const std::shared_ptr<Grid>& curr_grid = fes_grids.at(i);
                            const FragmentInfo::Fragment& curr_fragment = fragmentInfo.fragments.at(i);

                            FragGeo_SingleCoord* curr_singlecoorddata = dynamic_cast<FragGeo_SingleCoord*>(
                                    curr_fragment.relativeGeometries.at(
                                            FragmentRelativeGeometryType::SingleCoord).internal_data.get()
                            );

                            std::array<double, 3>& curr_coord = curr_singlecoorddata->coord;
                            Eigen::Vector3d coord_point = {curr_coord[0], curr_coord[1], curr_coord[2]};
                            Eigen::Vector3d rotated_point = x_rotate * y_rotate * z_rotate * coord_point;

                            for (int j = 0; j < 3; j++) {
                                coord_vector[j] += (rotated_point[j] / 10.0);
                            }
                            if (curr_grid->check_inrange(coord_vector)) {
                                candidate_fes_value += curr_grid->get_inrange(coord_vector);
                            }
                        }

                        if (fes_value < candidate_fes_value) {
                            fes_value = candidate_fes_value;
                        }

                    }
                }
            }
            result_grid->set_inrange(orig_coord_vector, fes_value);

            // Increment indexes until reaching the end
            if(!increment_grid_indexes(*result_grid, idx_in_bin_centers )) {
                break;
            }
        }
        BOOST_LOG_TRIVIAL(info) << "Done populating grid";


        return result_grid;


    }

    std::shared_ptr<Grid> reconstruction_verynaive_multiconfrotation_algorithm(const std::vector<std::shared_ptr<Grid>>& fes_grids,
                                                                               const std::vector<PlumedDatHeader>& fes_grid_headers,
                                                                               const FragmentInfo& fragmentInfo){
        BOOST_LOG_TRIVIAL(info) << "Creating result grid";

        // Make header with the minimal dimensions
        PlumedDatHeader result_header = grid_from_intersection_of_grids(fragmentInfo, fes_grid_headers);

        std::shared_ptr<Grid> result_grid = std::make_shared<Grid>(result_header);

        if(result_grid->num_dims != 3) {
            BOOST_LOG_TRIVIAL(info) << "Algorithm verynaive_multiconfrotation works with exactly three spatial coordinate";
            throw std::runtime_error("Incorrect number of dimension for algorithm");
        }


        BOOST_LOG_TRIVIAL(info) << "Populating grid";
        std::vector<double> coord_vector((size_t) result_grid->num_dims);
        std::vector<double> orig_coord_vector((size_t) result_grid->num_dims);
        std::vector<size_t> idx_in_bin_centers((size_t) result_grid->num_dims, 0);
        while(true) {

            // Populate grid
            for (int curr_dim = 0; curr_dim < result_grid->num_dims; curr_dim++) {
                orig_coord_vector[curr_dim] = result_grid->bin_center.at(curr_dim).at(idx_in_bin_centers.at(curr_dim));
            }
            coord_vector = orig_coord_vector;

            if (!result_grid->check_inrange(coord_vector)) {
                BOOST_LOG_TRIVIAL(error) << "result_grid->check_inrange failed for generated coord_vector = "
                                         << coord_vector;
                throw std::runtime_error("Coordinate vector not in range");
            }

            // TODO: insert equal conformer number check !!!

            const FragmentInfo::Fragment& dummy_fragment = fragmentInfo.fragments.front();
            FragGeo_MultipleConformerCoord* dummy_multicoord = dynamic_cast<FragGeo_MultipleConformerCoord*>(
                    dummy_fragment.relativeGeometries.at(
                            FragmentRelativeGeometryType::MultipleConformerCoord).internal_data.get()
            );

            assert(dummy_multicoord != nullptr);

            const int num_conformer = dummy_multicoord->coords.size();

            double fes_value = 0.0;
            const int num_subdiv_angle = 10;
            for (int curr_conformer_idx = 0; curr_conformer_idx < num_conformer; curr_conformer_idx++) {
                for (int x_step = 0; x_step < num_subdiv_angle; x_step++) {
                    const double curr_angle_x_radian = (2.0 * 3.14 / double(num_subdiv_angle)) * x_step;
                    Eigen::AngleAxis<double> x_rotate(curr_angle_x_radian,
                                                      Eigen::Vector3d(1.0, 0.0, 0.0));
                    for (int y_step = 0; x_step < num_subdiv_angle; x_step++) {
                        const double curr_angle_y_radian = (2.0 * 3.14 / double(num_subdiv_angle)) * y_step;
                        Eigen::AngleAxis<double> y_rotate(curr_angle_y_radian,
                                                          Eigen::Vector3d(0.0, 1.0, 0.0));
                        for (int z_step = 0; x_step < num_subdiv_angle; x_step++) {
                            const double curr_angle_z_radian = (2.0 * 3.14 / double(num_subdiv_angle)) * z_step;
                            Eigen::AngleAxis<double> z_rotate(curr_angle_z_radian,
                                                              Eigen::Vector3d(0.0, 0.0, 1.0));
                            double candidate_fes_value = 0.0;
                            for (int i = 0; i < fragmentInfo.fragments.size(); i++) {
                                coord_vector = orig_coord_vector;
                                const PlumedDatHeader& curr_header = fes_grid_headers.at(i);
                                const std::shared_ptr<Grid>& curr_grid = fes_grids.at(i);
                                const FragmentInfo::Fragment& curr_fragment = fragmentInfo.fragments.at(i);

                                FragGeo_MultipleConformerCoord* curr_singlecoorddata = dynamic_cast<FragGeo_MultipleConformerCoord*>(
                                        curr_fragment.relativeGeometries.at(
                                                FragmentRelativeGeometryType::MultipleConformerCoord).internal_data.get()
                                );

                                std::array<double, 3>& curr_coord = curr_singlecoorddata->coords[curr_conformer_idx];
                                Eigen::Vector3d coord_point = {curr_coord[0], curr_coord[1], curr_coord[2]};
                                Eigen::Vector3d rotated_point = x_rotate * y_rotate * z_rotate * coord_point;

                                for (int j = 0; j < 3; j++) {
                                    coord_vector[j] += (rotated_point[j] / 10.0);
                                }
                                if (curr_grid->check_inrange(coord_vector)) {
                                    candidate_fes_value += curr_grid->get_inrange(coord_vector);
                                }
                            }

                            if (fes_value < candidate_fes_value) {
                                fes_value = candidate_fes_value;
                            }

                        }
                    }
                }


            }

            result_grid->set_inrange(orig_coord_vector, fes_value);
            // Increment indexes until reaching the end
            if (!increment_grid_indexes(*result_grid, idx_in_bin_centers)) {
                break;
            }
        }
        std::cout << "\n";
        BOOST_LOG_TRIVIAL(info) << "Done populating grid";


        return result_grid;


    }

}