//
// Created by eliane on 26/03/2020.
//

#include "TempTextFile.hpp"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <boost/filesystem.hpp>

TempTextFile::TempTextFile(const std::string& content) {

    this->raw_content = std::vector(content.size() + 1, (uint8_t) 0x66);
    std::strcpy((char*) this->raw_content.data(), content.c_str());
    assert(std::strcmp((char*) this->raw_content.data(), content.c_str()) == 0);
    this->raw_content.resize(content.size());

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