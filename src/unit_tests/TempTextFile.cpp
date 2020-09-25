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

#include "TempTextFile.hpp"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <boost/filesystem.hpp>

TempTextFile::TempTextFile(const std::string& content) {

    this->processContent(content);

    #ifdef __linux__
        this->fds = fdopen(mkstemp(namebuf), "wb");
        this->name = namebuf;
    #else
        this->name = std::tmpnam(nullptr);
        this->fds = std::fopen(this->name.c_str(), "wb");
    #endif

    assert(this->fds != nullptr);
    assert(this->name != "");

    std::fputs(content.c_str(), this->fds);

    std::fflush(this->fds);

    this->filepath = this->name;
}

std::string TempTextFile::getName()  {
    return this->name;
}

const char* TempTextFile::getNameCStr() {
    return this->name.c_str();
}

TempTextFile::~TempTextFile() {
    std::fclose(this->fds);
    std::remove(this->name.c_str());

    // Check & remove bindumps:
    std::string potential_bindump_name = this->name + ".bindump";
    if(boost::filesystem::exists(potential_bindump_name)) {
        std::remove(potential_bindump_name.c_str());
    }


}

const std::vector<uint8_t>& TempTextFile::getRawData() {
    return this->raw_content;
}

TempTextFile::TempTextFile(const bf::path& directory, const std::string& name, const std::string& content) {
    this->processContent(content);


    this->filepath = directory;
    this->filepath /= name;

    this->name = this->filepath.generic_string();
    this->fds = std::fopen(this->name.c_str(), "wb");

    assert(this->fds != nullptr);
    assert(this->name != "");

    std::fputs(content.c_str(), this->fds);

    std::fflush(this->fds);

    this->filepath = this->name;

}

bf::path TempTextFile::getFilePath() {
    return this->filepath;
}

void TempTextFile::processContent(const std::string& content) {
    this->raw_content = std::vector(content.size() + 1, (uint8_t) 0x66);
    std::strcpy((char*) this->raw_content.data(), content.c_str());
    assert(std::strcmp((char*) this->raw_content.data(), content.c_str()) == 0);
    this->raw_content.resize(content.size());
}

TempTextFile::TempTextFile(const bf::path& directory, const std::string& name, std::vector<uint8_t> raw_content_) {
    this->filepath = directory;
    this->filepath /= name;

    this->name = this->filepath.generic_string();
    this->fds = std::fopen(this->name.c_str(), "wb");

    assert(this->fds != nullptr);
    assert(this->name != "");

    this->raw_content = std::move(raw_content_);

    size_t written = fwrite( this->raw_content.data(), sizeof(uint8_t), this->raw_content.size(), this->fds);

    if(written != this->raw_content.size()) {
        throw std::runtime_error("Cannot write raw content to temp file");
    }

    std::fflush(this->fds);

}
