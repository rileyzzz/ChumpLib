#include <iostream>
#include <vector>
#include <memory>

#ifndef _CHUMPFILE_H_
#define _CHUMPFILE_H_

class IOArchive;

enum class ChumpDataType : uint8_t
{
    Soup        = 0,
    Integer     = 1,
    Float       = 2,
    Text        = 3,
    Raw         = 4,
    Null        = 5,
    RawAligned  = 12,
    KUID        = 13
};

class ChumpChunk;

class ChumpData
{
private:
    //private constructor
    ChumpData() { }

public:
    virtual bool Serialize(class IOArchive& Ar, size_t datasize) = 0;

    friend class ChumpSoup;
    friend class ChumpInteger;
    friend class ChumpFloat;
    friend class ChumpText;
    friend class ChumpRaw;
    friend class ChumpNull;
    friend class ChumpKUID;
};

class ChumpSoup : public ChumpData
{
private:
    std::vector<ChumpChunk> children;

    //private constructor
    ChumpSoup() {}

public:
    bool Serialize(class IOArchive& Ar, size_t datasize);

    friend class ChumpChunk;
};

class ChumpInteger : public ChumpData
{
private:
    std::vector<int32_t> values;

    //private constructor
    ChumpInteger() { }

public:
    bool Serialize(class IOArchive& Ar, size_t datasize);

    friend class ChumpChunk;
};

class ChumpFloat : public ChumpData
{
private:
    std::vector<float> values;

    //private constructor
    ChumpFloat() { }

public:
    bool Serialize(class IOArchive& Ar, size_t datasize);

    friend class ChumpChunk;
};

class ChumpText : public ChumpData
{
private:
    std::string value;

    //private constructor
    ChumpText() { }

public:
    bool Serialize(class IOArchive& Ar, size_t datasize);

    friend class ChumpChunk;
};

class ChumpRaw : public ChumpData
{
    std::vector<int8_t> rawdata;
private:
    //private constructor
    ChumpRaw() { }

public:
    bool Serialize(class IOArchive& Ar, size_t datasize);

    friend class ChumpChunk;
};

class ChumpNull : public ChumpData
{
private:
    //private constructor
    ChumpNull() { }

public:
    bool Serialize(class IOArchive& Ar, size_t datasize) { return true; }

    friend class ChumpChunk;
};

//https://github.com/SilverGreen93/CDPExplorer/blob/master/kuid-format.md
#pragma pack(1)
struct KUIDdata
{
    int32_t userid : 25;
    int32_t contentid : 32;
    uint8_t version : 7;

    KUIDdata() : version(0), userid(0), contentid(0) {}
};
#pragma pack()

class ChumpKUID : public ChumpData
{
private:
    KUIDdata KUID;
    //private constructor
    ChumpKUID() { }

public:
    bool Serialize(class IOArchive& Ar, size_t datasize);

    friend class ChumpChunk;
};


class ChumpChunk
{
private:
    std::string chunkName;
    ChumpDataType chunkType;

    std::shared_ptr<ChumpData> data;

public:
    bool Serialize(class IOArchive& Ar);

};


class ChumpFile
{
private:

    uint32_t version = 1;
    uint32_t padding0 = 0;

    std::vector<ChumpChunk> rootData;

    ChumpFile() { }

    bool Serialize(class IOArchive& Ar);
public:
    static ChumpFile read(const char* path);
};
#endif // _CHUMPFILE_H_
