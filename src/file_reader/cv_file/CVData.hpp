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

namespace fesutils {

    class CVData {
    public:
        CVData(unsigned int num_cv_dimensions_,
               unsigned int num_value_dimensions_,
               size_t expected_number_record);

        virtual void insertDatapoint(const std::vector<double>& datapoint) = 0;
        virtual void insertDatapointsVector(const std::vector<double>& datapoint) = 0;

        virtual void printRecords(unsigned int begin_index, unsigned int num_records) = 0;

        virtual const unsigned int& get_num_cv_dimensions();

        virtual const unsigned int& get_num_value_dimensions();

        virtual const unsigned int& get_total_dim_in_one_record();

        virtual const size_t& get_num_record();

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

}

#endif //FESUTILS_CVDATA_HPP
