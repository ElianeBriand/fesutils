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

#include "parse_space_separated_values_node.hpp"

#include <boost/log/trivial.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/format.hpp>
#include <fmt/format.h>

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;


std::shared_ptr<Eigen::Array<double,
        Eigen::Dynamic,
        Eigen::Dynamic,
        Eigen::RowMajor>>
fesutils::parse_space_separated_double_node::operator()(const std::shared_ptr<std::vector<std::string>> lines_sptr) {
    this->dbg_call_to_operator++;
    size_t num_row = lines_sptr->size();


    auto array = std::make_shared<Eigen::Array<double,
            Eigen::Dynamic,
            Eigen::Dynamic,
            Eigen::RowMajor>>(num_row, this->num_columns);

    std::vector<double> line_data;
    line_data.reserve(this->num_columns);
    size_t curr_row_id  = 0;
    size_t curr_lines_block_idx = 0;
    for(const std::string& l : *lines_sptr) {
        curr_lines_block_idx++;
        this->processed_count++;
        line_data.clear();

        bool success = qi::phrase_parse(
                l.begin(),l.end(),
                qi::double_ % *qi::char_(' '),
                boost::spirit::ascii::space,
                line_data
        );



        if(line_data.size() != this->num_columns) {
            BOOST_LOG_TRIVIAL(debug) << "--------  BEGIN MALFORMED LINE DEBUG INFORMATION -------";
            BOOST_LOG_TRIVIAL(debug) << "Tokens: ";
            for(const auto& token: line_data) {
                BOOST_LOG_TRIVIAL(debug) << "   Token: " << token;
            }
            BOOST_LOG_TRIVIAL(debug) << "Success: " << success;

            this->error_count++;
            BOOST_LOG_TRIVIAL(debug) << "Nbr call to operator " << this->dbg_call_to_operator;
            BOOST_LOG_TRIVIAL(debug) << "Malformed line dropped: " << l;
            BOOST_LOG_TRIVIAL(debug) << "Index in block " << curr_lines_block_idx-1 << " of " << lines_sptr->size();
            BOOST_LOG_TRIVIAL(debug) << "Expected " << this->num_columns << " values but got " << line_data.size();
            BOOST_LOG_TRIVIAL(debug) << "Encountered " << this->error_count << " malformed line out of  " << this->processed_count
            << " (" << boost::format("%4.2f") % (float(this->error_count)/float(this->processed_count) * 100.0)  << "%)";
            BOOST_LOG_TRIVIAL(debug) << "Warning code 00003";
            BOOST_LOG_TRIVIAL(debug) << "Surrounding:";
            if(curr_lines_block_idx > 1) {
                BOOST_LOG_TRIVIAL(debug) << "Previous:";
                BOOST_LOG_TRIVIAL(debug) << lines_sptr->at( curr_lines_block_idx-2 );
            }
            if(curr_lines_block_idx < lines_sptr->size()) {
                BOOST_LOG_TRIVIAL(debug) << "Next:";
                BOOST_LOG_TRIVIAL(debug) << lines_sptr->at( curr_lines_block_idx );
            }
            BOOST_LOG_TRIVIAL(debug) << "--------  END MALFORMED LINE DEBUG INFORMATION -------";
            continue;
        }

        std::memcpy(array->row(curr_row_id).data(), line_data.data(), sizeof(double) * this->num_columns);
        curr_row_id++;
    }

    if(curr_row_id < num_row) {
        // Some row have been skipped due to lack of conforming line
        // Need to resize block
        auto resized_array = std::make_shared<Eigen::Array<double,
                Eigen::Dynamic,
                Eigen::Dynamic,
                Eigen::RowMajor>>(curr_row_id, this->num_columns);
        std::memcpy(resized_array->data(), array->data(), sizeof(double) * this->num_columns * curr_row_id);
        array = resized_array;
    }

    return array;
}

fesutils::parse_space_separated_double_node::parse_space_separated_double_node(size_t num_cols) :
        num_columns(num_cols),
        error_count(0),
        processed_count(0),
        dbg_call_to_operator(0)
        { }
