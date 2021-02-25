#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <chumplib.h>

void writeChunk(std::ofstream& str, ChumpChunk& chnk, int indentLevel = 0)
{
    const std::string spacer = " ";
    std::string indentOffset;
    for(int i = 0; i < indentLevel; i++)
        indentOffset += "\t";
    std::cout << "writing chunk " << chnk.chunkName << "\n";
    if(chnk.getChunkType() == ChumpDataType::Null)
        return;

    str << indentOffset << chnk.chunkName;
    std::shared_ptr<ChumpData> chnkdata = chnk.getData();
    switch(chnk.getChunkType())
    {
    case ChumpDataType::Soup:
    {
        auto data = std::dynamic_pointer_cast<ChumpSoup>(chnkdata);
        str << "\n" << indentOffset << "{\n";
        for(auto& child : data->children)
            writeChunk(str, child, indentLevel + 1);
        str << indentOffset << "}\n";
        break;
    }
    case ChumpDataType::Integer:
    {
        auto data = std::dynamic_pointer_cast<ChumpInteger>(chnkdata);
        str << spacer;
        for(int i = 0; i < data->values.size(); i++)
        {
            if(i > 0)
                str << ", ";

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
        for(int i = 0; i < data->values.size(); i++)
        {
            if(i > 0)
                str << ", ";

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

void writeConfig(std::ofstream& str, ChumpFile& file)
{
    for(auto& data : file.rootData)
        writeChunk(str, data);
}

int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        std::cout << "USAGE: chumpconv config.chump\n";
        return 0;
    }
    if(argc > 2 && strcmp(argv[1], "test") == 0)
    {
        ChumpFile testfile;
        ChumpChunk testChunk("key");
        testChunk.setData(std::make_shared<ChumpText>("value"));
        testfile.rootData.push_back(testChunk);
        testfile.save(argv[2]);
        return 0;
    }
    //const char* filename = argv[1];
    std::filesystem::path filepath(argv[1]);
    std::string outpath = filepath.replace_extension("config.txt").string();

    auto test = chump_read(argv[1]);

    std::ofstream out(outpath.c_str(), std::ofstream::out);
    writeConfig(out, test);
    out.close();
    
}
