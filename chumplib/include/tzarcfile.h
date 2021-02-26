#include <iostream>
#include <vector>
#include <memory>
#include <string>

#ifndef _TZARCFILE_H_
#define _TZARCFILE_H_

class IOArchive;

class TZarcObj
{

};

class TZarcFile
{
public:
    std::vector<TZarcObj> files;

private:
    uint32_t version = 1;

    bool Serialize(class IOArchive& Ar);
public:
    static TZarcFile read(const char* path);

    void save(const char* path);

    TZARCFile() { }
};
#endif // _TZARCFILE_H_
