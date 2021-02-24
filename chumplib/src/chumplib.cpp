#include "chumplib.h"
#include <iostream>
//#include <windows.h>

CHUMPLIB_API void chump_read(const char* file)
{
	std::cout << "neato\n";
	//ChumpFile::read(file);
    ChumpFile test;
    test.save("/workspace/ChumpLib/build/bin/test.chump");
    
	//MessageBoxA(0, "done", "done", MB_OK);
}
