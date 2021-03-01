#include <iostream>
#include <fstream>
//#include <filesystem>
//#include <experimental/filesystem>
#include <sys/stat.h>

#ifdef _MSC_VER
#include <direct.h>
#define mkdir(x) _mkdir(x)
#else
#define mkdir(x) mkdir(x, 0755)
#endif

#include <cstring>
#include <chumplib.h>


int main(int argc, char* argv[])
{
    ChumpFile::parseTXT("G:/Games/N3V/trs19/build hhl1hrpw1/editing/kuid -25 1173 class43 interior/config.txt");
    return 0;

    auto arc = tzarc_read(argv[1]);
    std::string outdir = argv[1];
    outdir = outdir.substr(0, outdir.find_last_of('.'));
    mkdir(outdir.c_str());

    std::cout << "read finished\n";
    for(const auto& file : arc.files)
    {
        std::string fpath = outdir + "/" + file.name;
        std::cout << "write to " << fpath << "\n";
        std::ofstream out(fpath.c_str(), std::ios::out | std::ios::binary);
        out.write(file.filedata, file.filesize);
        //out.write(reinterpret_cast<char*>(file.filedata), file.filesize);
        out.close();
    }
    return 0;

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
        //testfile.rootData.push_back(ChumpChunk);
        testfile.save(argv[2]);
        return 0;
    }
    //const char* filename = argv[1];
    //std::filesystem::path filepath(argv[1]);
    //std::string outpath = filepath.replace_extension(".txt").string();
    std::string outpath = argv[1];
    outpath = outpath.substr(0, outpath.find_last_of('.')) + ".txt";

    auto test = chump_read(argv[1]);

    test.exportTXT(outpath.c_str());
}
