
project(chumpconv VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(chumpconv chumpconv.cpp) # WIN32 windows subsystem
target_link_libraries(chumpconv LINK_PUBLIC chumplib)

#if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	#target_link_libraries(chumpconv LINK_PUBLIC chumplib stdc++fs)
#else()
	#target_link_libraries(chumpconv LINK_PUBLIC chumplib)
#endif()

add_dependencies(chumpconv chumplib)
