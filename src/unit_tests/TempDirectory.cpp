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

#include "TempDirectory.hpp"
#include <iostream>
#include <boost/filesystem.hpp>
namespace bf = boost::filesystem;

TempDirectory::TempDirectory() {

#ifdef __linux__
    mkdtemp(this->namebuf);
    this->name = this->namebuf;
#else
    this->name = std::tmpnam(nullptr);
    bool success = bf::create_directory(this->name);

    if(!success) {
        throw std::runtime_error("Cannot create temporary directory during test suite execution");
    }
#endif

    this->directory_path =  this->name;
}

TempDirectory::~TempDirectory() {
    this->files.clear();

    unsigned long num_deleted_file = bf::remove_all(this->name);
    if(num_deleted_file == 0) {
        std::cout << "WARNING/ERROR: Temporary directory could not be removed" << std::endl;
    }

    if(num_deleted_file != 1) {
        std::cout << "WARNING/ERROR: Some temporary files were still present before temporary directory removal" << std::endl;
    }
}

std::shared_ptr<TempTextFile> TempDirectory::createTemporaryFileInDir(const std::string& name_, const std::string& content) {
    this->files.emplace_back(std::make_shared<TempTextFile>(this->directory_path, name_, content));
    return this->files.back();
}

bf::path TempDirectory::getDirPath() {
    return this->directory_path;
}

std::string TempDirectory::getDirPathAsStr() {
    return this->name;
}

std::shared_ptr<TempTextFile>
TempDirectory::createTemporaryFileInDir(const std::string& name_, std::vector<uint8_t> raw_content) {
    this->files.emplace_back(std::make_shared<TempTextFile>(this->directory_path, name_, raw_content));
    return this->files.back();
}
