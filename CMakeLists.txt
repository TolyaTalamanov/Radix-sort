cmake_minimum_required(VERSION 2.8)




#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -gnu=c++11 ")

set(LIBRARY RadixSortsLib)
project(${LIBRARY} )

set(INCLUDE include/Sorts.h)
set(SOURCE  src/Sorts.cpp)
set(CMAKE_BUILD_TYPE Release)
add_definitions(-std=c++11)

#add_definitions(-Wall -O2)



#set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "Configs" FORCE)


#if(NOT CMAKE_BUILD_TYPE)
 #   set(CMAKE_BUILD_TYPE Release)
#endif()


find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
include_directories("include")
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
add_library(${CMAKE_PROJECT_NAME} STATIC ${INCLUDE} ${SOURCE})

add_subdirectory(sample)
#add_subdirectory(tests)
