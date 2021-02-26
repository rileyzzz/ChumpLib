#include <iostream>
#include <vector>
#include <memory>
#include <string>

#ifndef _TZARCFILE_H_
#define _TZARCFILE_H_

class IOArchive;

class TZArcFile
{
private:
    std::string name;
    uint32_t filesize;
    uint8_t* filedata;

    TZArcFile() : filesize(0), filedata(nullptr) {}

    friend class TZArchive;
};

class TZArchive
{
public:
    std::vector<TZArcFile> files;

private:
    uint32_t version = 1;

    bool Serialize(class IOArchive& Ar);
public:
    static TZArchive read(const char* path);

    void save(const char* path);

    TZArchive() { }
};
#endif // _TZARCFILE_H_
