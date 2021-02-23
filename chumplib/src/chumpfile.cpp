#include <iostream>
#include "chumpfile.h"
#include "IOArchive.h"

ChumpFile ChumpFile::read(const char* path)
{
    ChumpFile newFile;
    IOArchive Ar(path, IODirection::Import);
	if(!newFile.Serialize(Ar))
		std::cout << "Encountered file read error at offset " << Ar.tellg() << "\n";

    return newFile;
}

bool ChumpFile::Serialize(IOArchive& Ar)
{
    if(!Ar.ChunkHeader("ACS$"))
        return false;

    return true;
}
