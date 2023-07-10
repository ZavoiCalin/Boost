cmake_minimum_required( VERSION 3.2 ) 
project(Boost) 

#set( CMAKE_CXX_STANDARD 14 ) 
#set( CMAKE_CXX_STANDARD_REQUIRED ON ) 
#set( CMAKE_CXX_EXTENSIONS OFF ) 

set(CMAKE_BUILD_TYPE "Debug")

add_executable(main main.cpp)

add_library(Main main.h main.cpp)
target_include_directories(Main PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(main PRIVATE Main -lrt)
set( CMAKE_CXX_FLAGS " -pthread -lrt" )
