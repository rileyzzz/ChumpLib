#include "chumplib.h"
#include <iostream>
//#include <windows.h>

CHUMPLIB_API ChumpFile chump_read(const char* file)
{
	return ChumpFile::read(file);
    //ChumpFile test;
    //test.save("/workspace/ChumpLib/build/bin/test.chump");
    //test.save("C:/Users/10447696/Desktop/CB/projects/ChumpLib/test.chump");

	//MessageBoxA(0, "done", "done", MB_OK);
}

CHUMPLIB_API TZarcFile tzarc_read(const char* file)
{
	return TZarcFile::read(file);
}
