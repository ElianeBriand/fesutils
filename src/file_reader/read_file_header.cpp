//
// Created by eliane on 20/03/2020.
//

#include "read_file_header.hpp"

#include <fstream>
#include <tuple>

#include <boost/tokenizer.hpp>
#include <boost/log/trivial.hpp>


#include "../common/fs_utils.hpp"

namespace b = boost;

namespace fesutils {

    void parse_FIELDS_line(PlumedDatHeader& header, const std::string& line) {

        std::vector<std::string> tokens;

        b::char_separator<char> sep(" ");
        b::tokenizer<b::char_separator<char>> tok(line, sep);
        for(auto beg=tok.begin(); beg!=tok.end();++beg){
            tokens.push_back(*beg);
        }

        if(tokens.size() < 3) {
            BOOST_LOG_TRIVIAL(error) << "FIELDS header line: " << line;
            BOOST_LOG_TRIVIAL(error) << "contains no field names";
            throw std::runtime_error("Incorrect FIELDS header line");
        }

        for(int i = 2; i < tokens.size(); i++) {
            Field f;
            auto  [basename, subfield] = extract_basename_subfield_from_field_name(tokens.at(i));
            f.name = tokens.at(i);

            if(subfield == "") {
                f.is_subcomponent = false;
                f.basename = basename;
                f.subfield = "";
            }else {
                f.is_subcomponent = true;
                f.basename = basename;
                f.subfield = subfield;
            }
            header.fields.push_back(std::move(f));

        }

    }

    std::tuple<std::string, std::string> extract_basename_subfield_from_field_name(const std::string& field) {
        size_t pos_dot =  field.find('.');
        size_t pos_second_dot =  field.find('.', pos_dot+1);

        // If dot not found, or more than 1 dot, or no text after the dot
        if (pos_dot == std::string::npos ||
            pos_second_dot != std::string::npos ||
            pos_dot == field.size() - 1) {
            return std::make_tuple(field,"");
        }


        return std::make_tuple(field.substr(0,pos_dot),field.substr(pos_dot+1));
    }

    PlumedDatHeader read_cv_file_header(const std::string& path) {
        PlumedDatHeader header;

        if(!path_exists_isfile(path)) {
            BOOST_LOG_TRIVIAL(error) << "Path " << path << "cannot be opened: "  << path_notfile_whatistheproblem(path);
            throw std::runtime_error("Cannot open CV file");
        }

        std::ifstream inputf(path);

        std::string line;
        while(getline( inputf, line ) )
        {
            if(line.empty() || line[0] != '#') {
                break;
            }

            if(line.find("#! FIELDS ") != std::string::npos) {
                parse_FIELDS_line(header, line);
            } else {
                BOOST_LOG_TRIVIAL(debug) << "Header line was not used: " << line;
            }
        }


        return header;
    }

}