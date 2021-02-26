
#include "chumpfile.h"
#include "tzarcfile.h"

#ifndef _CHUMPLIB_H_
#define _CHUMPLIB_H_

#ifndef __has_declspec_attribute
  #define __has_declspec_attribute(x) 0
#endif

#if __has_declspec_attribute(dllexport)
#define CHUMPLIB_API __declspec(dllexport)
#else
#define CHUMPLIB_API
#endif

extern "C++" CHUMPLIB_API ChumpFile chump_read(const char* file);
extern "C++" CHUMPLIB_API TZArchive tzarc_read(const char* file);

#endif //_CHUMPLIB_H_
