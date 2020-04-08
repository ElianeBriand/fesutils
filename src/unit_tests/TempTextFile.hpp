//
// Created by eliane on 26/03/2020.
//

#ifndef FESUTILS_TEMPTEXTFILE_HPP
#define FESUTILS_TEMPTEXTFILE_HPP


#include <string>
#include <vector>

int constexpr length(const char* str)
{
    return *str ? 1 + length(str + 1) : 0;
}

class TempTextFile {
public:

    explicit TempTextFile(const std::string& content);
    ~TempTextFile();
    std::string getName();
    const char* getNameCStr();
    const std::vector<uint8_t>& getRawData();

private:
    char namebuf[length("test.XXXXXX") + 1] = "test.XXXXXX";
    std::string name;

    std::FILE* fds;

    std::vector<uint8_t> raw_content;
};


#endif //FESUTILS_TEMPTEXTFILE_HPP
