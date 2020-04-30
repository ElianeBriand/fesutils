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

#define BOOST_TEST_DYN_LINK

#include <vector>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>

#include <boost/test/unit_test.hpp>

#include "../../grid/NDArray.hpp"


namespace f = fesutils;
namespace tt = boost::test_tools;


BOOST_AUTO_TEST_SUITE(ndarray_ts)



    BOOST_AUTO_TEST_CASE(ndarray_factory_checkdims_tc) {

        std::shared_ptr<f::NDArray<double>> oneD =  f::NDArray_factory<double>({2});
        BOOST_TEST(oneD->get_dims() == 1);
        BOOST_TEST(oneD->check_dims(std::vector<long int>(1, 3)));
        BOOST_TEST(!oneD->check_dims(std::vector<long int>(12, 3)));

        std::shared_ptr<f::NDArray<double>> twoD =  f::NDArray_factory<double>({2, 2});
        BOOST_TEST(twoD->get_dims() == 2);
        BOOST_TEST(twoD->check_dims(std::vector<long int>(2, 3)));
        BOOST_TEST(!twoD->check_dims(std::vector<long int>(12, 3)));

        std::shared_ptr<f::NDArray<double>> threeD =  f::NDArray_factory<double>({2, 2, 2});
        BOOST_TEST(threeD->get_dims() == 3);
        BOOST_TEST(threeD->check_dims(std::vector<long int>(3, 3)));
        BOOST_TEST(!threeD->check_dims(std::vector<long int>(12, 3)));

        std::shared_ptr<f::NDArray<double>> fourD =  f::NDArray_factory<double>({2, 2, 2, 2});
        BOOST_TEST(fourD->get_dims() == 4);
        BOOST_TEST(fourD->check_dims(std::vector<long int>(4, 3)));
        BOOST_TEST(!fourD->check_dims(std::vector<long int>(12, 3)));

        BOOST_CHECK_THROW(f::NDArray_factory<double>({2, 2, 2, 2, 2}), std::runtime_error);

        BOOST_CHECK_THROW(f::NDArray_factory<double>({}), std::runtime_error);

    }

    BOOST_AUTO_TEST_CASE(set_zero_tc) {

        const long int dimension_size = 3;
        double targetValue = 0.0;
        std::vector<long int> indexes;

        std::shared_ptr<f::NDArray<double>> oneD =  f::NDArray_factory<double>(std::vector<long int>(1, dimension_size));
        oneD->zeroOut();
        for(int i = 0; i < dimension_size; i++) {
            //BOOST_TEST_CONTEXT("index " << i)
            {
                indexes = {i};
                BOOST_TEST(oneD->get(indexes) == targetValue);
                double val = -1.0;
                oneD->get(indexes, val);
                BOOST_TEST(val == targetValue);
                BOOST_TEST(oneD->operator[](indexes) == targetValue);
            }
        }


        std::shared_ptr<f::NDArray<double>> twoD =  f::NDArray_factory<double>(std::vector<long int>(2, dimension_size));
        twoD->zeroOut();
        for(int i = 0; i < dimension_size; i++) {
            for(int j = 0; j < dimension_size; j++) {
                //BOOST_TEST_CONTEXT("index " << i << "," << j)
                {
                    indexes = {i, j};
                    BOOST_TEST(twoD->get(indexes) == targetValue);
                    double val = -1.0;
                    twoD->get(indexes, val);
                    BOOST_TEST(val == targetValue);
                    BOOST_TEST(twoD->operator[](indexes) == targetValue);
                }
            }
        }


        std::shared_ptr<f::NDArray<double>> threeD =  f::NDArray_factory<double>(std::vector<long int>(3, dimension_size));
        threeD->zeroOut();
        for(int i = 0; i < dimension_size; i++) {
            for(int j = 0; j < dimension_size; j++) {
                for(int k = 0; k < dimension_size; k++) {
                    //BOOST_TEST_CONTEXT("index " << i << "," << j << "," << k)
                    {
                        indexes = {i, j, k};
                        BOOST_TEST(threeD->get(indexes) == targetValue);
                        double val = -1.0;
                        threeD->get(indexes, val);
                        BOOST_TEST(val == targetValue);
                        BOOST_TEST(threeD->operator[](indexes) == targetValue);
                    }
                }
            }
        }

        std::shared_ptr<f::NDArray<double>> fourD =  f::NDArray_factory<double>(std::vector<long int>(4, dimension_size));
        fourD->zeroOut();
        for(int i = 0; i < dimension_size; i++) {
            for(int j = 0; j < dimension_size; j++) {
                for(int k = 0; k < dimension_size; k++) {
                    for(int l = 0; l < dimension_size; l++) {
                        //BOOST_TEST_CONTEXT("index " << i << "," << j << "," << k << "," << l)
                        {
                            indexes = {i, j, k, l};
                            BOOST_TEST(fourD->get(indexes) == targetValue);
                            double val = -1.0;
                            fourD->get(indexes, val);
                            BOOST_TEST(val == targetValue);
                            BOOST_TEST(fourD->operator[](indexes) == targetValue);
                        }
                    }
                }
            }
        }

    }

    BOOST_AUTO_TEST_CASE(set_value_tc) {
        std::shared_ptr<f::NDArray<double>> oneD =  f::NDArray_factory<double>({2});
        oneD->set({1}, 124.3);
        BOOST_TEST(oneD->get({1}) == 124.3);

        std::shared_ptr<f::NDArray<double>> twoD =  f::NDArray_factory<double>({2, 2});
        twoD->set({1, 1}, 124.3);
        BOOST_TEST(twoD->get({1, 1}) == 124.3);

        std::shared_ptr<f::NDArray<double>> threeD =  f::NDArray_factory<double>({2, 2, 2});
        threeD->set({1, 1, 1}, 124.3);
        BOOST_TEST(threeD->get({1, 1, 1}) == 124.3);

        std::shared_ptr<f::NDArray<double>> fourD =  f::NDArray_factory<double>({2, 2, 2, 2});
        fourD->set({1, 1, 1, 1}, 124.3);
        BOOST_TEST(fourD->get({1, 1, 1, 1}) == 124.3);

    }

    BOOST_AUTO_TEST_CASE(set_all_value_tc) {

        const long int dimension_size = 3;
        double targetValue = 123.1;
        std::vector<long int> indexes;

        std::shared_ptr<f::NDArray<double>> oneD =  f::NDArray_factory<double>(std::vector<long int>(1, dimension_size));
        oneD->zeroOut();
        oneD->setAllToValue(targetValue);
        for(int i = 0; i < dimension_size; i++) {
            //BOOST_TEST_CONTEXT("index " << i)
            {
                indexes = {i};
                BOOST_TEST(oneD->get(indexes) == targetValue);
                double val = -1.0;
                oneD->get(indexes, val);
                BOOST_TEST(val == targetValue);
                BOOST_TEST(oneD->operator[](indexes) == targetValue);
            }
        }


        std::shared_ptr<f::NDArray<double>> twoD =  f::NDArray_factory<double>(std::vector<long int>(2, dimension_size));
        twoD->zeroOut();
        twoD->setAllToValue(targetValue);
        for(int i = 0; i < dimension_size; i++) {
            for(int j = 0; j < dimension_size; j++) {
                //BOOST_TEST_CONTEXT("index " << i << "," << j)
                {
                    indexes = {i, j};
                    BOOST_TEST(twoD->get(indexes) == targetValue);
                    double val = -1.0;
                    twoD->get(indexes, val);
                    BOOST_TEST(val == targetValue);
                    BOOST_TEST(twoD->operator[](indexes) == targetValue);
                }
            }
        }


        std::shared_ptr<f::NDArray<double>> threeD =  f::NDArray_factory<double>(std::vector<long int>(3, dimension_size));
        threeD->zeroOut();
        threeD->setAllToValue(targetValue);
        for(int i = 0; i < dimension_size; i++) {
            for(int j = 0; j < dimension_size; j++) {
                for(int k = 0; k < dimension_size; k++) {
                    //BOOST_TEST_CONTEXT("index " << i << "," << j << "," << k)
                    {
                        indexes = {i, j, k};
                        BOOST_TEST(threeD->get(indexes) == targetValue);
                        double val = -1.0;
                        threeD->get(indexes, val);
                        BOOST_TEST(val == targetValue);
                        BOOST_TEST(threeD->operator[](indexes) == targetValue);
                    }
                }
            }
        }

        std::shared_ptr<f::NDArray<double>> fourD =  f::NDArray_factory<double>(std::vector<long int>(4, dimension_size));
        fourD->zeroOut();
        fourD->setAllToValue(targetValue);
        for(int i = 0; i < dimension_size; i++) {
            for(int j = 0; j < dimension_size; j++) {
                for(int k = 0; k < dimension_size; k++) {
                    for(int l = 0; l < dimension_size; l++) {
                        //BOOST_TEST_CONTEXT("index " << i << "," << j << "," << k << "," << l)
                        {
                            indexes = {i, j, k, l};
                            BOOST_TEST(fourD->get(indexes) == targetValue);
                            double val = -1.0;
                            fourD->get(indexes, val);
                            BOOST_TEST(val == targetValue);
                            BOOST_TEST(fourD->operator[](indexes) == targetValue);
                        }
                    }
                }
            }
        }

    }

    BOOST_AUTO_TEST_CASE(raw_ptr_tc) {

        const long int dimension_size = 3;
        double targetValue = 123.1;
        std::vector<long int> indexes;

        std::vector<double> reference_data(dimension_size*dimension_size*dimension_size*dimension_size, targetValue);
        int memcmp_res = 0;

        std::shared_ptr<f::NDArray<double>> oneD =  f::NDArray_factory<double>(std::vector<long int>(1, dimension_size));
        oneD->zeroOut();
        oneD->setAllToValue(targetValue);
        size_t bytelength_ref = dimension_size * sizeof(double);
        BOOST_TEST(bytelength_ref == oneD->get_raw_data_bytelength());
        BOOST_REQUIRE(bytelength_ref == oneD->get_raw_data_bytelength());
        memcmp_res = std::memcmp(oneD->get_raw_data_ptr(), (char*) reference_data.data(), bytelength_ref);
        BOOST_TEST(memcmp_res == 0);

        std::shared_ptr<f::NDArray<double>> twoD =  f::NDArray_factory<double>(std::vector<long int>(2, dimension_size));
        twoD->zeroOut();
        twoD->setAllToValue(targetValue);
        bytelength_ref = dimension_size * dimension_size * sizeof(double);
        BOOST_TEST(bytelength_ref == twoD->get_raw_data_bytelength());
        BOOST_REQUIRE(bytelength_ref == twoD->get_raw_data_bytelength());
        memcmp_res = std::memcmp(twoD->get_raw_data_ptr(), (char*) reference_data.data(), bytelength_ref);
        BOOST_TEST(memcmp_res == 0);


        std::shared_ptr<f::NDArray<double>> threeD =  f::NDArray_factory<double>(std::vector<long int>(3, dimension_size));
        threeD->zeroOut();
        threeD->setAllToValue(targetValue);
        bytelength_ref = dimension_size * dimension_size * dimension_size * sizeof(double);
        BOOST_TEST(bytelength_ref == threeD->get_raw_data_bytelength());
        BOOST_REQUIRE(bytelength_ref == threeD->get_raw_data_bytelength());
        memcmp_res = std::memcmp(threeD->get_raw_data_ptr(), (char*) reference_data.data(), bytelength_ref);
        BOOST_TEST(memcmp_res == 0);

        std::shared_ptr<f::NDArray<double>> fourD =  f::NDArray_factory<double>(std::vector<long int>(4, dimension_size));
        fourD->zeroOut();
        fourD->setAllToValue(targetValue);
        bytelength_ref = dimension_size * dimension_size * dimension_size * dimension_size * sizeof(double);
        BOOST_TEST(bytelength_ref == fourD->get_raw_data_bytelength());
        BOOST_REQUIRE(bytelength_ref == fourD->get_raw_data_bytelength());
        memcmp_res = std::memcmp(fourD->get_raw_data_ptr(), (char*) reference_data.data(), bytelength_ref);
        BOOST_TEST(memcmp_res == 0);

    }


BOOST_AUTO_TEST_SUITE_END();