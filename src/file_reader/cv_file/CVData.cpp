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

    void InMemoryCVData::getDatapoint(size_t index, std::vector<double>& datapoint_buffer) {
        if(index >= this->number_of_record)
        {
            BOOST_LOG_TRIVIAL(error) << "InMemoryCVData::getDatapoint: out of bound access, index = " << index << " but number of record = " << this->number_of_record;
            throw std::runtime_error("Trying to access out-of bound CVData datapoint");
        }
        const size_t array_index = this->total_dim_in_one_record * index;
        std::copy(this->cv_data_array.begin() + array_index,this->cv_data_array.begin() + array_index + this->total_dim_in_one_record, datapoint_buffer.begin());
    }

    std::vector<double> InMemoryCVData::getDatapoint(size_t index) {
        if(index >= this->number_of_record)
        {
            BOOST_LOG_TRIVIAL(error) << "InMemoryCVData::getDatapoint: out of bound access, index = " << index << " but number of record = " << this->number_of_record;
            throw std::runtime_error("Trying to access out-of bound CVData datapoint");
        }
        std::vector<double> datapoint;
        const size_t array_index = this->total_dim_in_one_record * index;
        datapoint.insert(datapoint.begin(), this->cv_data_array.begin() + array_index,this->cv_data_array.begin() + array_index + this->total_dim_in_one_record);
        return datapoint;
    }



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


    CVData_span::CVData_span(CVData* underlying_cvdata_, size_t begin_offset_, long int requested_span_size_) :
            underlying_cvdata(std::move(underlying_cvdata_)),
            begin_offset(begin_offset_),
            requested_span_size(requested_span_size_)
    {
        if(this->underlying_cvdata == nullptr) {
            throw std::runtime_error("CVData_span constructor called with null pointer");
        }

        const size_t underlying_cvdata_size = this->underlying_cvdata->get_num_record();
        if(this->begin_offset > underlying_cvdata_size) {
            BOOST_LOG_TRIVIAL(error) << "CVData_span constructor called with offset beyond end of CVData";
            BOOST_LOG_TRIVIAL(error) << "CVData->size = " << underlying_cvdata_size
            << " | begin_offset = " << this->begin_offset << " | requested_span_size = " << this->requested_span_size;
            throw std::runtime_error("CVData_span constructor called with offset beyond end of CVData");
        }

        if(this->requested_span_size == -1) {
            // -1 codes for remainder of the CVData
            this->actual_size = underlying_cvdata_size - this->begin_offset;
            this->end_offset = this->begin_offset + this->actual_size;
        } else if(this->requested_span_size < -1) {
            BOOST_LOG_TRIVIAL(error) << "CVData_span constructor called with requested_span_size = " << this->requested_span_size
            << " which is not a valid span size (allowed size -1, 0 and positive integer)";
            throw std::runtime_error("CVData_span constructor called with invalid requested_span_size");
        } else {
            // Actual size of span requested
            if( ( this->begin_offset + this->requested_span_size) > underlying_cvdata_size) {
                // The span extends past the end: reduce it to fill the remaining space instead
                this->actual_size = underlying_cvdata_size - this->begin_offset;
                this->end_offset = this->begin_offset + this->actual_size;
            } else {
                this->actual_size = this->requested_span_size;
                this->end_offset = this->begin_offset + this->requested_span_size;
            }
        }


    }

    bool CVData_span::is_requested_span_size() const {
        if(this->requested_span_size == -1)
            return true;
        else
            return this->actual_size == this->requested_span_size;
    }

    size_t CVData_span::get_begin_offset() const {
        return begin_offset;
    }

    size_t CVData_span::get_end_offset() const {
        return end_offset;
    }

    size_t CVData_span::get_actual_size() const {
        return actual_size;
    }


}