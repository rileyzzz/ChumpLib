#include <iostream>
#include <bitset>
#include "chumpfile.h"
#include "IOArchive.h"

void writeChunk(std::ofstream& str, ChumpChunk& chnk, int indentLevel = 0)
{
    std::string indentOffset;
    for (int i = 0; i < indentLevel; i++)
        indentOffset += "    ";
    std::cout << "writing chunk " << chnk.chunkName << "\n";
    if (chnk.getChunkType() == ChumpDataType::Null || chnk.getChunkType() == ChumpDataType::Raw || chnk.getChunkType() == ChumpDataType::RawAligned)
        return;

    //Trainz does this
    if (chnk.getChunkType() == ChumpDataType::Soup && indentLevel == 0)
        str << "\n";

    str << indentOffset << chnk.chunkName;

    std::string spacer = " ";
    int spacesLeft = 40 - (indentOffset.size() + chnk.chunkName.size());
    if (spacesLeft > 1)
    {
        for (int i = 1; i < spacesLeft; i++)
            spacer += " ";
    }

    std::shared_ptr<ChumpData> chnkdata = chnk.getData();
    switch (chnk.getChunkType())
    {
    case ChumpDataType::Soup:
    {
        auto data = std::dynamic_pointer_cast<ChumpSoup>(chnkdata);
        str << "\n" << indentOffset << "{\n";
        for (auto& child : data->children)
            writeChunk(str, child, indentLevel + 1);
        str << indentOffset << "}\n";
        break;
    }
    case ChumpDataType::Integer:
    {
        auto data = std::dynamic_pointer_cast<ChumpInteger>(chnkdata);
        str << spacer;
        for (int i = 0; i < data->values.size(); i++)
        {
            if (i > 0)
                str << ",";

            const auto& val = data->values[i];
            str << val;
        }
        str << "\n";
        break;
    }
    case ChumpDataType::Float:
    {
        auto data = std::dynamic_pointer_cast<ChumpFloat>(chnkdata);
        str << spacer;
        for (int i = 0; i < data->values.size(); i++)
        {
            if (i > 0)
                str << ",";

            const auto& val = data->values[i];
            str << val;
        }
        str << "\n";
        break;
    }
    case ChumpDataType::Text:
    {
        auto data = std::dynamic_pointer_cast<ChumpText>(chnkdata);
        str << spacer << "\"" << data->value << "\"\n";
        break;
    }
    case ChumpDataType::KUID:
    {
        auto data = std::dynamic_pointer_cast<ChumpKUID>(chnkdata);
        str << spacer << data->KUID.KUIDstr() << "\n";
        break;
    }
    default:
        std::cout << "unknown chunk type " << (int)chnk.getChunkType() << "\n";
        break;
    }
}

ChumpFile ChumpFile::read(const char* path)
{
    ChumpFile newFile;
    IOArchive Ar(path, IODirection::Import);
	if(!newFile.Serialize(Ar))
		std::cout << "Encountered file read error at offset " << Ar.tell() << "\n";

    return newFile;
}

void ChumpFile::save(const char* path)
{
    IOArchive Ar(path, IODirection::Export);
	if(!Serialize(Ar))
		std::cout << "Encountered file write error at offset " << Ar.tell() << "\n";
}

void ChumpFile::exportTXT(const char* path)
{
    std::ofstream out(path, std::ofstream::out);
    for (auto& data : rootData)
        writeChunk(out, data);
    out.close();
}

bool ChumpFile::Serialize(IOArchive& Ar)
{
    if(!Ar.ChunkHeader("ACS$"))
        return false;

    Ar << version;
    Ar << padding0;

    //use tellg etc after serialization to replace this value when saving
    int32_t filelength = 0;
    if (Ar.IsSaving())
    {
        for (auto& child : rootData)
            filelength += child.size();
    }
    Ar << filelength;

    //std::cout << "filelength " << filelength << "\n";

    std::streampos dataStart = Ar.tell();
    //std::cout << "datastart " << dataStart << "\n";

    if (Ar.IsLoading())
    {
        while ((size_t)(Ar.tell() - dataStart) < filelength)
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
    uint8_t strLength = str.size() + 1;
    Ar << strLength;

    if (Ar.IsLoading())
    {
        char* strData = new char[strLength] {};
        Ar.Serialize(strData, strLength);
        str = strData;
        delete[] strData;
    }
    else
    {
        Ar.Serialize(str.data(), std::streamsize(strLength));
    }
}

int ChumpChunk::size()
{
    int chunkSize = 0;
    chunkSize += sizeof(int32_t); //pairlength
    chunkSize += chunkName.size() + 2;
    chunkSize += 1; //chunkType
    chunkSize += data->size();
    return chunkSize;
}

bool ChumpChunk::Serialize(IOArchive& Ar)
{
    //std::cout << "chunk start " << Ar.tell() << "\n";

    int32_t pairLength = 0;
    if (Ar.IsSaving())
    {
        pairLength += chunkName.size() + 2;
        pairLength += 1; //chunkType
        pairLength += data->size();
    }
    Ar << pairLength;

    std::streampos pairStart = Ar.tell();

    SerializeString(Ar, chunkName);
    Ar << chunkType;
    
    //std::cout << "data begin " << (int)Ar.tellg() << " name " << chunkName << " type " << (int)chunkType << "\n";

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
    int dataLeft = pairLength - (Ar.tell() - pairStart);
    //std::cout << "data left " << dataLeft << "\n";
    if(dataLeft > 0 || Ar.IsSaving())
        data->Serialize(Ar, dataLeft);

    //std::cout << "chunk length " + std::to_string(size()) << " actual " + std::to_string(pairLength + 4) << "\n";

    return true;
}

bool ChumpSoup::Serialize(IOArchive& Ar, size_t datasize)
{
    std::streampos pairStart = Ar.tell();

    if (Ar.IsLoading())
    {
        while (Ar.tell() - pairStart < datasize)
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

int ChumpSoup::size()
{
    int size = 0;
    for (auto& child : children)
        size += child.size();

    return size;
}

bool ChumpInteger::Serialize(IOArchive& Ar, size_t datasize)
{
    std::streampos dataStart = Ar.tell();

    if (Ar.IsLoading())
    {
        values.resize(datasize / sizeof(int32_t));
        Ar.Serialize(values.data(), datasize);
    }
    else
    {
        Ar.Serialize(values.data(), values.size() * sizeof(int32_t));
    }
    //std::cout << "int data " << value << "\n";
    for(const auto& value : values)
        std::cout << value << ", ";
    std::cout << "\n";
    return true;
}

bool ChumpFloat::Serialize(IOArchive& Ar, size_t datasize)
{
    std::streampos dataStart = Ar.tell();

    if (Ar.IsLoading())
    {
        values.resize(datasize / sizeof(float));
        Ar.Serialize(values.data(), datasize);
    }
    else
    {
        Ar.Serialize(values.data(), values.size() * sizeof(float));
    }

    //std::cout << "float data " << value << "\n";
        for(const auto& value : values)
        std::cout << value << ", ";
    std::cout << "\n";

    return true;
}

bool ChumpText::Serialize(IOArchive& Ar, size_t datasize)
{
    if (Ar.IsLoading())
    {
        char* str = new char[datasize];
        Ar.Serialize(str, datasize);
        value = str;
        delete[] str;
    }
    else
    {
        Ar.Serialize(value.data(), value.size() + 1);
    }

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
    //Ar << KUID._raw;
    //sizeof(KUID._raw);
    Ar << KUID._high;
    Ar << KUID._low;

    if ((KUID.version & 0b1111111) == 0b1111111)
        KUID.version = 0;

    //std::cout << "kuidsize " << sizeof(KUIDdata) << "\n";
    //std::cout << "kuid data userid " << (int)KUID.userid << " contentid " << (int)KUID.contentid << " version " << (int)KUID.version << "\n";
    //std::cout << "userid " << (int)KUID.userid << " contentid " << (int)KUID.contentid << " version " << (int)KUID.version << "\n";
    //std::cout << std::bitset<7>(KUID.version) << "\n";
    //std::cout << std::bitset<32>(KUID._high) << "\n";
    //std::cout << std::bitset<25>(KUID.userid) << "\n";
    //std::cout << std::bitset<7>(KUID.version) << "\n";
    //std::cout << std::bitset<32>(KUID._low) << "\n";
    std::cout << KUID.KUIDstr() << "\n";
    return true;
}
