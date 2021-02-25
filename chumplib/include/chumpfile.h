#include <iostream>
#include <vector>
#include <memory>
#include <string>

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
    virtual int size() = 0;

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
public:
    std::vector<ChumpChunk> children;

private:
    //private constructor
    ChumpSoup() {}

public:
    bool Serialize(class IOArchive& Ar, size_t datasize);
    int size();

    friend class ChumpChunk;
};

class ChumpInteger : public ChumpData
{
public:
    std::vector<int32_t> values;

private:
    //private constructor
    ChumpInteger() { }

public:
    bool Serialize(class IOArchive& Ar, size_t datasize);
    int size() { return values.size() * sizeof(int32_t); }

    friend class ChumpChunk;
};

class ChumpFloat : public ChumpData
{
public:
    std::vector<float> values;

private:
    //private constructor
    ChumpFloat() { }

public:
    bool Serialize(class IOArchive& Ar, size_t datasize);
    int size() { return values.size() * sizeof(float); }

    friend class ChumpChunk;
};

class ChumpText : public ChumpData
{
public:
    std::string value;

private:
    //private constructor
    ChumpText() { }

public:
    bool Serialize(class IOArchive& Ar, size_t datasize);
    int size() { return value.size() + 1; }

    friend class ChumpChunk;
};

class ChumpRaw : public ChumpData
{
public:
    std::vector<int8_t> rawdata;

private:
    //private constructor
    ChumpRaw() { }

public:
    bool Serialize(class IOArchive& Ar, size_t datasize);
    int size() { return rawdata.size(); }

    friend class ChumpChunk;
};

class ChumpNull : public ChumpData
{
private:
    //private constructor
    ChumpNull() { }

public:
    bool Serialize(class IOArchive& Ar, size_t datasize) { return true; }
    int size() { return 0; }

    friend class ChumpChunk;
};

//https://github.com/SilverGreen93/CDPExplorer/blob/master/kuid-format.md

struct KUIDdata
{
public:
#pragma pack(1)
    union
    {
        struct
        {
            signed userid : 25;
            unsigned version : 7;
        };
        uint32_t _high;
    };
    union
    {
        signed contentid : 32;
        uint32_t _low;
    };
#pragma pack()
public:
    KUIDdata() : _high(0), _low(0) {}
    inline std::string KUIDstr()
    {
        return std::string() + "<kuid" + (version > 0 ? "2:" : ":") + std::to_string(userid) + ":" + std::to_string(contentid) + ((version > 0) ? (":" + std::to_string(version)) : "") + ">";
    }
    friend class ChumpKUID;
};

class ChumpKUID : public ChumpData
{
public:
    KUIDdata KUID;

private:
    //private constructor
    ChumpKUID() { }

public:
    bool Serialize(class IOArchive& Ar, size_t datasize);
    int size() { return 8; }
    friend class ChumpChunk;
};


class ChumpChunk
{
public:
    std::string chunkName;

private:
    ChumpDataType chunkType;
    std::shared_ptr<ChumpData> data;

public:
    bool Serialize(class IOArchive& Ar);
    int size();

    std::shared_ptr<ChumpData> getData() { return data; }
    const ChumpDataType getChunkType() { return chunkType; }
    friend class ChumpSoup;
};


class ChumpFile
{
public:
    std::vector<ChumpChunk> rootData;

private:
    uint32_t version = 1;
    uint32_t padding0 = 0;

    bool Serialize(class IOArchive& Ar);
public:
    static ChumpFile read(const char* path);

    void save(const char* path);

    ChumpFile() { }
};
#endif // _CHUMPFILE_H_
