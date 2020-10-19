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


#ifndef FESUTILS_CVDATA_HPP
#define FESUTILS_CVDATA_HPP

#include <vector>
#include <memory>

namespace fesutils {

    class CVData {
    public:
        CVData(unsigned int num_cv_dimensions_,
               unsigned int num_value_dimensions_,
               size_t expected_number_record);

        virtual void insertDatapoint(const std::vector<double>& datapoint) = 0;
        virtual void insertDatapointsVector(const std::vector<double>& datapoint) = 0;

        /**
         * Copy datapoint at index offset to datapoint_buffer. The size of datapoint_buffer must
         * be at least get_total_dim_in_one_record(). The index must be lower than get_num_record().
         *
         *
         * \param index zero-based index of the record.
         * \param datapoint_buffer
         * \sa get_total_dim_in_one_record, get_num_record
         */
        virtual void getDatapoint(size_t index, std::vector<double>& datapoint_buffer) const  = 0;

        /**
         * Return datapoint at index offset. The index must be lower than get_num_record().
         *
         * \param index zero-based index of the record.
         * \sa get_num_record
         */
        virtual std::vector<double> getDatapoint(size_t index) = 0;

        virtual void printRecords(unsigned int begin_index, unsigned int num_records) = 0;

        [[nodiscard]] virtual const unsigned int& get_num_cv_dimensions() const;

        [[nodiscard]] virtual const unsigned int& get_num_value_dimensions() const;

        [[nodiscard]] virtual const unsigned int& get_total_dim_in_one_record() const;

        [[nodiscard]] virtual const size_t& get_num_record() const;

    protected:
        unsigned int num_cv_dimensions;
        unsigned int num_value_dimensions;
        unsigned int total_dim_in_one_record;

        size_t number_of_record;

    };

    class InMemoryCVData : public CVData {
    public:
        InMemoryCVData(unsigned int num_cv_dimensions_,
                       unsigned int num_value_dimensions_,
                       size_t expected_number_record = 1000);

        ~InMemoryCVData();

        void insertDatapoint(const std::vector<double>& datapoint) final;

        void insertDatapointsVector(const std::vector<double>& datapoint) final;

        void getDatapoint(size_t index, std::vector<double>& datapoint_buffer) const final;

        std::vector<double> getDatapoint(size_t index) final;

        void printRecords(unsigned int begin_index, unsigned int num_records) final;

    protected:

        std::vector<double> cv_data_array;

    };

    enum class CVData_storage_class {
        inmemory,
        diskandcache,
        disk
    };

    std::shared_ptr<CVData> CVData_factory(CVData_storage_class cv_storage_class,
                                           unsigned int num_cv_dimensions_,
                                           unsigned int num_value_dimensions_,
                                           size_t expected_number_record);

    class CVData_span {
    public:

        CVData_span(CVData* underlying_cvdata,
                    size_t begin_offset,
                    long int requested_span_size);

        [[nodiscard]] bool is_requested_span_size() const;

        [[nodiscard]] size_t get_begin_offset() const;
        [[nodiscard]] size_t get_end_offset() const;
        [[nodiscard]] size_t get_actual_size() const;

    private:
        CVData* underlying_cvdata;
        size_t begin_offset;
        long int requested_span_size;

        size_t end_offset;
        size_t actual_size;
    };



}

#endif //FESUTILS_CVDATA_HPP
