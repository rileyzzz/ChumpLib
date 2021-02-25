#include <iostream>
#include <chumplib.h>
int main(int argc, char* argv[])
{
    //chump_read("C:/Users/10447696/Desktop/TRS19/Install/resources/builtin/46400/config-cache.chump");
    //chump_read("E:/Program Files/N3V Games/Trainz Railroad Simulator 2019/resources/builtin/46400/config-cache.chump");
    auto test = chump_read("C:/Users/riley/Desktop/Trainz/trainzcore/kuid2 414976 102484 2/config.chump");

    test.save("C:/Users/riley/Desktop/Trainz/trainzcore/kuid2 414976 102484 2/config2.chump");
    //getchar();
}
