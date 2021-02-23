
#ifndef _CHUMPLIB_H_
#define _CHUMPLIB_H_

#define CHUMPLIB_API __declspec(dllexport)

extern "C" CHUMPLIB_API void chump_read(const char* filename);

#endif