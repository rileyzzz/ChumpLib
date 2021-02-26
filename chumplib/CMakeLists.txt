
project(chumplib VERSION 1.0)

set(LIBRARY_HEADERS
	include/chumplib.h
	include/chumpfile.h
	include/tzarcfile.h)

set(LIBRARY_SOURCES
	src/chumplib.cpp
	src/chumpfile.cpp
	src/tzarcfile.cpp
	src/IOArchive.h)

add_library(chumplib STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})

target_include_directories(chumplib PUBLIC include)
target_include_directories(chumplib PRIVATE src)
#set_target_properties(chumplib PROPERTIES PUBLIC_HEADER include/chumplib.h)

#source_group(chumplib REGULAR_EXPRESSION src/*.*)