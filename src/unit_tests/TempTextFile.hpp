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

#ifndef FESUTILS_TEMPTEXTFILE_HPP
#define FESUTILS_TEMPTEXTFILE_HPP


#include <string>
#include <vector>

#include <boost/filesystem.hpp>
namespace bf = boost::filesystem;


int constexpr constexpr_charbuf_length(const char* str){
    return *str ? 1 + constexpr_charbuf_length(str + 1) : 0;
}


class TempTextFile {
public:

    explicit TempTextFile(const std::string& content);
    TempTextFile(const bf::path& directory, const std::string& name, const std::string& content);
    TempTextFile(const bf::path& directory, const std::string& name, std::vector<uint8_t> raw_content);

    ~TempTextFile();
    std::string getName();
    const char* getNameCStr();
    const std::vector<uint8_t>& getRawData();

    bf::path getFilePath();

private:
    void processContent(const std::string& content);

    char namebuf[constexpr_charbuf_length("test.XXXXXX") + 1] = "test.XXXXXX";
    std::string name;

    bf::path filepath;

    std::FILE* fds;

    std::vector<uint8_t> raw_content;
};


#endif //FESUTILS_TEMPTEXTFILE_HPP
