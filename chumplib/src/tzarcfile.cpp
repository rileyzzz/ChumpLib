#include <iostream>

#ifdef _MSC_VER
#include <intrin.h>
#define bswap_32(x) _byteswap_ulong(x)
#define bswap_64(x) _byteswap_uint64(x)
#else
#include <x86intrin.h>
#define bswap_32(x) __builtin_bswap32(x)
#define bswap_64(x) __builtin_bswap64(x)
#endif

#include "tzarcfile.h"
#include "IOArchive.h"

TZArchive TZArchive::read(const char* path)
{
    TZArchive newFile;
    IOArchive Ar(path, IODirection::Import);
	if(!newFile.Serialize(Ar))
		std::cout << "Encountered file read error at offset " << Ar.tell() << "\n";

    return newFile;
}

void SerializeCondensedString(IOArchive& Ar, std::string& str)
{
    uint8_t strLength = str.size();
    Ar << strLength;

    if (Ar.IsLoading())
    {
        char* strData = new char[strLength + 1] {};
        Ar.Serialize(strData, strLength);
        str = strData;
        delete[] strData;
    }
    else
    {
        Ar.Serialize(str.data(), std::streamsize(strLength));
    }
}

bool TZArchive::Serialize(IOArchive& Ar)
{
    if(!Ar.ChunkHeader("TZac"))
        return false;

    version = bswap_32(version);
    Ar << version;
    version = bswap_32(version);
    std::cout << "File version " << version << "\n";

    uint32_t filecount = files.size();
    filecount = bswap_32(filecount);
    Ar << filecount;
    filecount = bswap_32(filecount);
    std::cout << filecount << " files\n";

    if(Ar.IsLoading())
        files.resize(filecount);

    for(int i = 0; i < filecount; i++)
    {
        auto& file = files[i];
        SerializeCondensedString(Ar, file.name);

        //compression?
        uint32_t unknown = 0;
        Ar << unknown;
        unknown = bswap_32(unknown);
        std::cout << "unknown value " << unknown << "\n";

        file.filesize = bswap_32(file.filesize);
        Ar << file.filesize;
        file.filesize = bswap_32(file.filesize);

        std::cout << "file " << file.name << " size " << file.filesize << "\n";
    }

    //crc?
    uint32_t unknown2 = 0;
    Ar << unknown2;
    unknown2 = bswap_32(unknown2);
    std::cout << "unknown 2 " << unknown2 << "\n";

    //read file data
    for(int i = 0; i < filecount; i++)
    {
        auto& file = files[i];
        file.filedata = new char[file.filesize];
        Ar.Serialize(file.filedata, file.filesize);
        //std::cout << "read file data for " << file.name << ", fstream position " << Ar.tell() << "\n";
    }
    return true;
}
