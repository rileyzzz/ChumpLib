cmake_minimum_required(VERSION 3.10)

project(chumplib VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)

set(LIBRARY_HEADERS
	include/chumplib.h)

set(LIBRARY_SOURCES
	src/chumplib.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(src)
add_library(chumplib STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})

set_target_properties(chumplib PROPERTIES PUBLIC_HEADER include/chumplib.h)
target_include_directories(chumplib PRIVATE include)
target_include_directories(chumplib PRIVATE src)

#project(chumpconv VERSION 1.0)

#add_subdirectory(chumpconv)

#target_include_directories(chumpconv PUBLIC
#                          "${PROJECT_BINARY_DIR}"
#                          )

#source_group(chumpconv REGULAR_EXPRESSION chumpconv/*.*)