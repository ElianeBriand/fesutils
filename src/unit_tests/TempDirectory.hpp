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

#ifndef FESUTILS_TEMPDIRECTORY_HPP
#define FESUTILS_TEMPDIRECTORY_HPP



#include <vector>
#include <memory>
#include "TempTextFile.hpp"

class TempDirectory {
public:
    TempDirectory();
    ~TempDirectory();

    std::shared_ptr<TempTextFile> createTemporaryFileInDir(const std::string& name, const std::string& content);
    std::shared_ptr<TempTextFile> createTemporaryFileInDir(const std::string& name, std::vector<uint8_t> raw_content);
    std::string getDirPathAsStr();
    bf::path getDirPath();

private:
    char namebuf[constexpr_charbuf_length("testdir.XXXXXX") + 1] = "testdir.XXXXXX";
    std::string name;

    bf::path directory_path;

    std::vector<std::shared_ptr<TempTextFile>> files;

};


#endif //FESUTILS_TEMPDIRECTORY_HPP
