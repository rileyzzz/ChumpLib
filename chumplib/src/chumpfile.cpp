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

    Ar << version;
    Ar << padding0;

    //use tellg etc after serialization to replace this value when saving
    int32_t filelength = 0;
    Ar << filelength;

    std::cout << filelength << "\n";

    std::streampos dataStart = Ar.tellg();

    if (Ar.IsLoading())
    {
        while ((size_t)(Ar.tellg() - dataStart) < filelength)
        {
            ChumpChunk newChunk;
            if (!newChunk.Serialize(Ar))
            {
                rootData.clear();
                return false;
            }
            rootData.push_back(newChunk);
        }
    }
    else
    {
        for (auto& child : rootData)
        {
            if (!child.Serialize(Ar))
                return false;
        }
    }

    return true;
}

void SerializeString(IOArchive& Ar, std::string& str)
{
    uint8_t strLength = str.size();
    Ar << strLength;
    char* strData = new char[strLength] {}; //initialize to 0

    if (Ar.IsSaving())
        strcpy(strData, str.data());

    Ar.Serialize(strData, strLength);

    if (Ar.IsLoading())
        str = strData;

    delete[] strData;
}


bool ChumpChunk::Serialize(IOArchive& Ar)
{
    //std::cout << "chunk start " << Ar.tellg() << "\n";
    int32_t pairLength = 0;
    Ar << pairLength;

    std::streampos pairStart = Ar.tellg();

    SerializeString(Ar, chunkName);
    Ar << chunkType;

    //std::cout << "data begin " << Ar.tellg() << " name " << chunkName << " type " << (int)chunkType << "\n";

    if (Ar.IsLoading())
    {
        switch (chunkType)
        {
        case ChumpDataType::Soup:
            data = std::shared_ptr<ChumpData>(new ChumpSoup());
            break;
        case ChumpDataType::Integer:
            data = std::shared_ptr<ChumpData>(new ChumpInteger());
            break;
        case ChumpDataType::Float:
            data = std::shared_ptr<ChumpData>(new ChumpFloat());
            break;
        case ChumpDataType::Text:
            data = std::shared_ptr<ChumpData>(new ChumpText());
            break;
        case ChumpDataType::Raw:
            data = std::shared_ptr<ChumpData>(new ChumpRaw());
            break;
        case ChumpDataType::Null:
            data = std::shared_ptr<ChumpData>(new ChumpNull());
            break;
        case ChumpDataType::KUID:
            data = std::shared_ptr<ChumpData>(new ChumpKUID());
            break;

        default:
            std::cout << "Unsupported chump type " << (uint32_t)chunkType << "!\n";
            assert(false);
            break;
        }
    }

    std::cout << chunkName << " ";
    //std::cout << "read chunk " << chunkName << " type " << (int)chunkType << "\n";

    assert(data);
    int dataLeft = pairLength - (Ar.tellg() - pairStart);
    //std::cout << "data left " << dataLeft << "\n";
    if(dataLeft > 0)
        data->Serialize(Ar, dataLeft);

    return true;
}

bool ChumpSoup::Serialize(IOArchive& Ar, size_t datasize)
{
    std::streampos pairStart = Ar.tellg();

    if (Ar.IsLoading())
    {
        while (Ar.tellg() - pairStart < datasize)
        {
            ChumpChunk newChunk;
            if(!newChunk.Serialize(Ar))
            {
                children.clear();
                return false;
            }
            children.push_back(newChunk);
            //std::cout << "data left " << datasize - (Ar.tellg() - pairStart);
        }
    }
    else
    {
        for (auto& child : children)
        {
            if (!child.Serialize(Ar))
                return false;
        }
    }

    return true;
}

bool ChumpInteger::Serialize(IOArchive& Ar, size_t datasize)
{
    Ar << value;

    //std::cout << "int data " << value << "\n";
    std::cout << value << "\n";
    return true;
}

bool ChumpFloat::Serialize(IOArchive& Ar, size_t datasize)
{
    Ar << value;

    //std::cout << "float data " << value << "\n";
    std::cout << value << "\n";
    return true;
}

bool ChumpText::Serialize(IOArchive& Ar, size_t datasize)
{
    char* str;

    if (Ar.IsLoading())
    {
        str = new char[datasize];
    }
    else
    {
        str = new char[value.size()];
        strcpy(str, value.data());
    }

    Ar.Serialize(str, datasize);

    if (Ar.IsLoading())
        value = str;

    delete[] str;

    //std::cout << "text data " << value << "\n";
    std::cout << value << "\n";
    return true;
}

bool ChumpRaw::Serialize(IOArchive& Ar, size_t datasize)
{
    if (Ar.IsLoading())
        rawdata.resize(datasize);

    Ar.Serialize(rawdata.data(), rawdata.size());

    return true;
}

bool ChumpKUID::Serialize(IOArchive& Ar, size_t datasize)
{
    Ar << KUID;
    //std::cout << "kuid data userid " << (int)KUID.userid << " contentid " << (int)KUID.contentid << " version " << (int)KUID.version << "\n";
    std::cout << "userid " << (int)KUID.userid << " contentid " << (int)KUID.contentid << " version " << (int)KUID.version << "\n";
    return true;
}
