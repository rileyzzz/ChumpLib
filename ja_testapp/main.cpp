#include <stdio.h>
#include <fstream>
#include <cstring>

#define READ(str, a) str.read(reinterpret_cast<char*>(&a), sizeof(a))

int main(int argc, char* argv[])
{
    if(argc <= 1)
    {
        printf("usage: testapp path_to_CFIL\n");
        return 0;
    }

    //char* Buffer1;
    char* Buffer2;
    int Count2 = 0;
    char* Temp1 = new char[256];
    char* Temp2 = new char[256];
    char* Temp3 = new char[256];
    char* Temp4 = new char[256];
    unsigned char TempChar = 0;
    int Counter = 0, Temp = 0, Temp2value = 0;
    short BlockSize = 0;
    
    int TempIt = 0;
    //uncompressed size
    Buffer2 = new char[1000];
    memset(Buffer2, 0x20, 1000);

    std::ifstream str(argv[1], std::ios::in | std::ios::binary);
    str.seekg(0, std::ios::end);
    size_t filesize = str.tellg();
    str.seekg(0x4, std::ios::beg); //CFIL

    //str >> BlockSize;
    READ(str, BlockSize);

    printf("blocksize %d\n", BlockSize);
    //while(!str.eof())
    {
        for(int i = 0; i < 256; i++)
            Temp1[i] = Temp3[i] = i;
        
        READ(str, TempChar);
        Temp = 0;
        printf("initial char %d\n", TempChar);

        while(Temp < 256)
        {
            if(TempChar > 0x7F)
            {
                Temp += TempChar - 0x7F;
                printf("temp increment, add %d value %d\n", TempChar, Temp);
            }
            else
            {
                printf("temp value %d, char %d\n", Temp, TempChar);
                for(int i = 0; i < TempChar + 1; i++)
                {
                    READ(str, Temp1[Temp]);
                    if(Temp != Temp1[Temp])
                        READ(str, Temp4[Temp]);
                    
                    Temp++;
                }
            }

            if(Temp < 256)
                READ(str, TempChar);
        }

        //read next two characters, swap
        int a = 0, b = 0;
        str.read(reinterpret_cast<char*>(&a), 1);
        str.read(reinterpret_cast<char*>(&b), 1);
        if(str.eof())
            printf("EOF!\n");
        //READ(str, a);
        //READ(str, b);
        TempIt = (a << 8) | b;
        printf("a %ud\n", a);
        printf("b %ud\n", b);
        printf("tempit %d\n", TempIt);
        
        Temp2value = 0;

        while(true)
        {
            if(Temp2value != 0)
            {
                Temp2value--;
                Temp = Temp2[Temp2value];
            }
            else
            {
                printf("temp2val 0! TempIt %d\n", TempIt);
                Temp = TempIt;
                TempIt--;
                if(Temp == 0)
                    break;

                char t = 0;
                READ(str, t);
                Temp = t;
                //READ(str, Temp);
            }

            TempChar = Temp1[Temp];
            if(Temp == (TempChar & 0xFF))
            {
                //hacky prevent segfault
                //if(Count2 > 1000) break;

                //write to final
                //printf("write character %c\n", Temp);
                Buffer2[Count2] = Temp;
                Count2++;
            }
            else
            {
                printf("temp2\n");
                Temp = Temp4[Temp];
                Temp2[Temp2value] = Temp;
                Temp2[Temp2value + 1] = TempChar;
                Temp2value += 2;
            }
        }
    }
    str.close();

    std::ofstream out("/workspace/ChumpLib/ja_testapp/build/bin/out.txt", std::ios::out | std::ios::binary);
    out.write(Buffer2, 1000);
    out.close();
    return 0;
}