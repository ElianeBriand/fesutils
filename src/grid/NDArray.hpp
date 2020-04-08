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

#ifndef FESUTILS_NDARRAY_HPP
#define FESUTILS_NDARRAY_HPP

#include <vector>
#include <memory>

#include <unsupported/Eigen/CXX11/Tensor>

namespace fesutils {

    //template <unsigned int dim>
    template<typename ValueType>
    class NDArray {
    public:

        virtual long int get_dims() = 0;
        virtual bool check_dims(const std::vector<long int>& indexes) = 0;

        virtual void set(const std::vector<long int>& indexes, const ValueType& value) = 0;

        virtual ValueType get(const std::vector<long int>& indexes) = 0;
        virtual void get(const std::vector<long int>& indexes, ValueType& value) = 0;

        virtual ValueType& operator[] (const std::vector<long int>& indexes) = 0;

        virtual void zeroOut() = 0;
        virtual void setAllToValue(const ValueType& value) = 0;
    };

    template<typename ValueType>
    class OneDArray : public NDArray<ValueType> {
    public:
        explicit OneDArray(const std::array<long int, 1>& dims) :
                tensor(dims[0]) {};

        virtual long int get_dims() {
            return 1;
        }

        virtual bool check_dims(const std::vector<long int>& indexes) {
            return indexes.size() == 1;
        }

        virtual void set(const std::vector<long int>& indexes, const ValueType& value) {
            tensor(indexes[0]) = value;
        }

        virtual ValueType get(const std::vector<long int>& indexes) {
            return tensor(indexes[0]);
        }

        virtual void get(const std::vector<long int>& indexes, ValueType& value) {
            value = tensor(indexes[0]);
        }

        virtual ValueType& operator[] (const std::vector<long int>& indexes)  {
            return tensor(indexes[0]);
        }

        virtual void zeroOut() {
            this->tensor.setConstant(0.0);
        }

        virtual void setAllToValue(const ValueType& value) {
            this->tensor.setConstant(value);
        }

    private:
        Eigen::Tensor<ValueType, 1> tensor;
    };

    template<typename ValueType>
    class TwoDArray : public NDArray<ValueType> {
    public:
        explicit TwoDArray(const std::array<long int, 2>& dims) :
                tensor(dims[0], dims[1]) {};

        virtual long int get_dims() {
            return 2;
        }

        virtual bool check_dims(const std::vector<long int>& indexes) {
            return indexes.size() == 2;
        }

        virtual void set(const std::vector<long int>& indexes, const ValueType& value) {
            tensor(indexes[0], indexes[1]) = value;
        }

        virtual ValueType get(const std::vector<long int>& indexes) {
            return tensor(indexes[0], indexes[1]);
        }

        virtual void get(const std::vector<long int>& indexes, ValueType& value) {
            value = tensor(indexes[0], indexes[1]);
        }

        virtual ValueType& operator[] (const std::vector<long int>& indexes)  {
            return tensor(indexes[0], indexes[1]);
        }

        virtual void zeroOut() {
            this->tensor.setConstant(0.0);
        }

        virtual void setAllToValue(const ValueType& value) {
            this->tensor.setConstant(value);
        }

    private:
        Eigen::Tensor<ValueType, 2> tensor;
    };


    template<typename ValueType>
    class ThreeDArray : public NDArray<ValueType> {
    public:
        explicit ThreeDArray(const std::array<long int, 3>& dims) :
                tensor(dims[0], dims[1], dims[2]) {};

        virtual long int get_dims() {
            return 3;
        }

        virtual bool check_dims(const std::vector<long int>& indexes) {
            return indexes.size() == 3;
        }

        virtual void set(const std::vector<long int>& indexes, const ValueType& value) {
            tensor(indexes[0], indexes[1], indexes[2]) = value;
        }

        virtual ValueType get(const std::vector<long int>& indexes) {
            return tensor(indexes[0], indexes[1], indexes[2]);
        }

        virtual void get(const std::vector<long int>& indexes, ValueType& value) {
            value = tensor(indexes[0], indexes[1], indexes[2]);
        }

        virtual ValueType& operator[] (const std::vector<long int>& indexes)  {
            return tensor(indexes[0], indexes[1], indexes[2]);
        }

        virtual void zeroOut() {
            this->tensor.setConstant(0.0);
        }

        virtual void setAllToValue(const ValueType& value) {
            this->tensor.setConstant(value);
        }

    private:
        Eigen::Tensor<ValueType, 3> tensor;
    };



    template<typename ValueType>
    class FourDArray : public NDArray<ValueType> {
    public:
        explicit FourDArray(const std::array<long int, 4>& dims) :
                tensor(dims[0], dims[1], dims[2], dims[3]) {};

        virtual long int get_dims() {
            return 4;
        }

        virtual bool check_dims(const std::vector<long int>& indexes) {
            return indexes.size() == 4;
        }

        virtual void set(const std::vector<long int>& indexes, const ValueType& value) {
            tensor(indexes[0], indexes[1], indexes[2], indexes[3]) = value;
        }

        virtual ValueType get(const std::vector<long int>& indexes) {
            return tensor(indexes[0], indexes[1], indexes[2], indexes[3]);
        }

        virtual void get(const std::vector<long int>& indexes, ValueType& value) {
            value = tensor(indexes[0], indexes[1], indexes[2], indexes[3]);
        }

        virtual ValueType& operator[] (const std::vector<long int>& indexes)  {
            return tensor(indexes[0], indexes[1], indexes[2], indexes[3]);
        }

        virtual void zeroOut() {
            this->tensor.setConstant(0.0);
        }

        virtual void setAllToValue(const ValueType& value) {
            this->tensor.setConstant(value);
        }

    private:
        Eigen::Tensor<ValueType, 4> tensor;
    };


    template<typename ValueType>
    std::shared_ptr<NDArray<ValueType>> NDArray_factory(const std::vector<long int>& dims) {
        std::shared_ptr<NDArray<ValueType>> ret;

        if(dims.size() == 1) {
            std::array<long int, 1> dims_arr = {dims[0]};
            ret = std::make_shared<OneDArray<ValueType>>(dims_arr);
        }else if (dims.size() == 2) {
            std::array<long int, 2> dims_arr = {dims[0], dims[1]};
            ret = std::make_shared<TwoDArray<ValueType>>(dims_arr);
        }else if (dims.size() == 3) {
            std::array<long int, 3> dims_arr = {dims[0], dims[1], dims[2]};
            ret = std::make_shared<ThreeDArray<ValueType>>(dims_arr);
        }else if (dims.size() == 4) {
            std::array<long int, 4> dims_arr = {dims[0], dims[1], dims[2], dims[3]};
            ret = std::make_shared<FourDArray<ValueType>>(dims_arr);
        }else {
            throw std::runtime_error("Data structure for " + std::to_string(dims.size()) + "-dimensional grid is not implemented.");
        }

        return ret;
    }


}



#endif //FESUTILS_NDARRAY_HPP
