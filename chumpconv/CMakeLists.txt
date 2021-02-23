
project(chumpconv VERSION 1.0)
add_executable(chumpconv WIN32 chumpconv.cpp)
target_link_libraries(chumpconv LINK_PUBLIC chumplib)

