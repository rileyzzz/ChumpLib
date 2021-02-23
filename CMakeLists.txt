cmake_minimum_required(VERSION 3.10)

project(ChumpLibrary)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)


add_subdirectory(chumpconv)

#source_group(chumpconv REGULAR_EXPRESSION chumpconv/*.*)


add_subdirectory(chumplib)

include_directories(${PROJECT_SOURCE_DIR}/chumplib/include)



#source_group(chumplib REGULAR_EXPRESSION src/*.*)

#target_include_directories(chumplib PRIVATE src)

#add_subdirectory(src)


#add_subdirectory(chumpconv)

#target_include_directories(chumpconv PUBLIC
#                          "${PROJECT_BINARY_DIR}"
#                          )

