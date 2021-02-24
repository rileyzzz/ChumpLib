
project(chumpconv VERSION 1.0)
add_executable(chumpconv chumpconv.cpp) # WIN32 windows subsystem
target_link_libraries(chumpconv LINK_PUBLIC chumplib)

