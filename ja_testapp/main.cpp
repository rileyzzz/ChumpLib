#include <stdio.h>
#include <fstream>

int main(int argc, char* argv[])
{
    if(argc <= 1)
    {
        printf("usage: testapp path_to_CFIL\n");
        return 0;
    }

    char* Buffer1, Buffer2;
    int Count, Count2;
    char* Temp1 = new char[256];
    char* Temp2 = new char[256];
    char* Temp3 = new char[256];
    char* Temp4 = new char[256];
    int TempChar, Counter, Temp, Temp2value, TempIt;
    short BlockSize;

    std::ifstream str(argv[1], std::ios::in | std::ios::binary);
    str.seekg(0, std::ios::end);
    size_t filesize = str.tellg();
    str.seekg(0x4, std::ios::beg); //CFIL

    str >> BlockSize;

    printf("blocksize %d\n", BlockSize);


    str.close();
    return 0;
}