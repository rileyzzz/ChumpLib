#include <iostream>
#include <fstream>
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
        //return 0;
    }
    //const char* filename = argv[1];

    //chump_read("C:/Users/10447696/Desktop/TRS19/Install/resources/builtin/46400/config-cache.chump");
    //chump_read("E:/Program Files/N3V Games/Trainz Railroad Simulator 2019/resources/builtin/46400/config-cache.chump");
    //auto test = chump_read("C:/Users/riley/Desktop/Trainz/trainzcore/kuid2 414976 102484 2/config.chump");
    //auto test = chump_read("E:/Program Files/N3V Games/Trainz Railroad Simulator 2019/resources/builtin/46400/config-cache.chump");

    auto test = chump_read("C:/Users/10447696/Desktop/TRS19/Install/resources/builtin/46400/config-cache.chump");

    std::ofstream out("C:/Users/10447696/Desktop/TRS19/Install/resources/builtin/46400/config-cache.config.txt", std::ofstream::out);
    writeConfig(out, test);
    out.close();
    //test.save("C:/Users/riley/Desktop/Trainz/trainzcore/kuid2 414976 102484 2/config2.chump");
    //getchar();

}
