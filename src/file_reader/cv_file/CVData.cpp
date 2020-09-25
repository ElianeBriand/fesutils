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


#include "CVData.hpp"
#include "../../common/NotImplementedError.hpp"


#include <boost/log/trivial.hpp>

namespace fesutils {

    CVData::CVData(unsigned int num_cv_dimensions_,
                   unsigned int num_value_dimensions_,
                   size_t expected_number_record) :
            num_cv_dimensions(num_cv_dimensions_),
            num_value_dimensions(num_value_dimensions_)
    {
        this->total_dim_in_one_record = num_cv_dimensions + num_value_dimensions;
        this->number_of_record = 0;
    }


    InMemoryCVData::InMemoryCVData(unsigned int num_cv_dimensions_,
                                   unsigned int num_value_dimensions_,
                                   size_t expected_number_record) :
            CVData(num_cv_dimensions_, num_value_dimensions_, expected_number_record )
    {


        try {
            this->cv_data_array.reserve(expected_number_record);

            // LCOV_EXCL_START
            // Reason for coverage exclusion: Cannot easily unit test lack of memory
        } catch(const std::bad_alloc& e) {
            BOOST_LOG_TRIVIAL(error) << "Failed in allocating memory for " <<  expected_number_record << " expected record.";
            BOOST_LOG_TRIVIAL(error) << "This is around " <<  expected_number_record * this->total_dim_in_one_record  * sizeof(double) << " bytes";
            BOOST_LOG_TRIVIAL(error) << "If this is greater than RAM, keeping all CV data in memory may not be possible";
            BOOST_LOG_TRIVIAL(error) << "Consider using disk-backed CV storage.";
            BOOST_LOG_TRIVIAL(debug) << "Bad alloc message: " << e.what();
            throw std::runtime_error("Not enough memory");
            // LCOV_EXCL_STOP
        }


    }

    const unsigned int& CVData::get_num_cv_dimensions() {
        return this->num_cv_dimensions;
    }

    const unsigned int& CVData::get_num_value_dimensions() {
        return this->num_value_dimensions;
    }

    const unsigned int& CVData::get_total_dim_in_one_record() {
        return this->total_dim_in_one_record;
    }

    const size_t& CVData::get_num_record() {
        return this->number_of_record;
    }


    InMemoryCVData::~InMemoryCVData() = default;

    void InMemoryCVData::insertDatapoint(const std::vector<double>& datapoint) {
        if(datapoint.size() != this->total_dim_in_one_record) {
            BOOST_LOG_TRIVIAL(error) << "InMemoryCVData::insertDatapoint : tried to insert a data point with incorrect dimensions";
            BOOST_LOG_TRIVIAL(error) << "InMemoryCVData::insertDatapoint : Expected " << this->num_cv_dimensions  << " (CV) + "
            << this->num_value_dimensions<< " (values)";
            BOOST_LOG_TRIVIAL(error) << "InMemoryCVData::insertDatapoint : Got " << datapoint.size();
            throw std::runtime_error("Wrong datapoint dimensions");
        }

        this->cv_data_array.insert(this->cv_data_array.end(), datapoint.begin(), datapoint.end());
        this->number_of_record++;

    }

    void InMemoryCVData::insertDatapointsVector(const std::vector<double>& datapoints) {
        if(datapoints.size() % this->total_dim_in_one_record != 0) {
            BOOST_LOG_TRIVIAL(error) << "InMemoryCVData::insertDatapointsVector: Number of element in data packet not compatible with expected number of dims in each record";
            BOOST_LOG_TRIVIAL(error) << "InMemoryCVData::insertDatapointsVector: datapoints.size() = " << datapoints.size() << " | this->total_dim_in_one_record = " << this->total_dim_in_one_record;
            BOOST_LOG_TRIVIAL(error) << "InMemoryCVData::insertDatapointsVector: data.size() % this->total_dim_in_one_record = " << datapoints.size() % this->total_dim_in_one_record;
            throw std::runtime_error("Data packet dimensions incompatible");
        }

        this->cv_data_array.insert(this->cv_data_array.end(), datapoints.begin(), datapoints.end());
        this->number_of_record += datapoints.size() / this->total_dim_in_one_record;

    }

    // LCOV_EXCL_START
    // Reason for coverage exclusion: Difficult to test pure stdio functions. Also not important for processing itself
    void InMemoryCVData::printRecords(unsigned int begin_index, unsigned int num_records) {
        if(begin_index >= this->number_of_record) {
            BOOST_LOG_TRIVIAL(error) << "InMemoryCVData::printRecords: begin index out of bound (starting index is " << begin_index << " but there are only " << this->number_of_record << " records)";
            throw std::runtime_error("Data packet dimensions incompatible");
        }

        int num_record_actually_printable = this->number_of_record - begin_index;
        if(num_records > num_record_actually_printable) {
            BOOST_LOG_TRIVIAL(warning) << "InMemoryCVData::printRecords: tried to print " << num_records << " but there are only " << num_record_actually_printable  << " records after index " << begin_index;
            num_records = num_record_actually_printable;
        }

        for(unsigned int i = 0; i < num_records; i++) {
            std::string values;
            for(unsigned int j = 0; j < this->total_dim_in_one_record; j++) {
                values += std::to_string(this->cv_data_array.at((begin_index* this->total_dim_in_one_record) + (i * this->total_dim_in_one_record) + j));
                values += "   ";
            }
            BOOST_LOG_TRIVIAL(info) << values;
        }

    }
    // LCOV_EXCL_STOP


    std::shared_ptr<CVData> CVData_factory(CVData_storage_class cv_storage_class,
                                           unsigned int num_cv_dimensions_,
                                           unsigned int num_value_dimensions_,
                                           size_t expected_number_record) {
        std::shared_ptr<CVData> new_cvdata_object;

        if(cv_storage_class == fesutils::CVData_storage_class::inmemory) {
            new_cvdata_object = std::make_shared<InMemoryCVData>(num_cv_dimensions_, num_value_dimensions_, expected_number_record);
        } else if (cv_storage_class == fesutils::CVData_storage_class::diskandcache) {
            BOOST_LOG_TRIVIAL(error) << "[NOTIMPLEMENTED] diskandcache storage class for CV data is not implemented";
            throw not_implemented_error("CVData_storage_class::diskandcache not implemented.");
        } else if (cv_storage_class == fesutils::CVData_storage_class::disk) {
            BOOST_LOG_TRIVIAL(error) << "[NOTIMPLEMENTED] disk storage class for CV data is not implemented";
            throw not_implemented_error("CVData_storage_class::disk not implemented.");
        }else {
            // LCOV_EXCL_START
            // Reason for coverage exclusion: Difficult to generate incorrect values for enum class
            BOOST_LOG_TRIVIAL(error) << "Unknown CV storage class enum value";
            throw std::runtime_error("Unknown CV storage class");
            // LCOV_EXCL_STOP
        }

        return new_cvdata_object;
    }



}