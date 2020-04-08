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

#include "extract_from_string.hpp"


#include <boost/log/trivial.hpp>
#include <boost/tokenizer.hpp>
#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;
namespace b = boost;

namespace fesutils {

    std::vector<float> extract_range_from_string(const std::string& str) {
        std::vector<float> range;

        bool success = qi::phrase_parse(
                str.begin(),str.end(),
                qi::float_ % qi::char_(','),
                boost::spirit::ascii::space,
                range
        );


        return range;
    }


    std::vector<std::string> slow_tokenize(const std::string& line) {
        std::vector<std::string> tokens;

        b::char_separator<char> sep(" ");
        b::tokenizer<b::char_separator<char>> tok(line, sep);
        for(auto beg=tok.begin(); beg!=tok.end();++beg){
            tokens.push_back(*beg);
        }

        return tokens;
    }

    bool parse_bool_as_word(const std::string& s) {
        if(s == "true") {
            return true;
        }
        if(s == "false") {
            return false;
        }
        throw std::runtime_error("Encountered unparseable boolean value: " + s + " (Error code 00002)");
        return false;
    }


}

