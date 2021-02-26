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

TZarcFile TZarcFile::read(const char* path)
{
    TZarcFile newFile;
    IOArchive Ar(path, IODirection::Import);
	if(!newFile.Serialize(Ar))
		std::cout << "Encountered file read error at offset " << Ar.tell() << "\n";

    return newFile;
}

bool TZarcFile::Serialize(IOArchive& Ar)
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

    }
    return true;
}
