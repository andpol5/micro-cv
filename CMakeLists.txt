cmake_minimum_required(VERSION 2.8)
project(microcv)
 
# Try to find the Boost libraries and includes automagically
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS filesystem regex system thread date_time REQUIRED)
if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIR})
	link_directories(${Boost_LIBRARY_DIR})
endif()

# Set C++11 flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# Header files from this project
include_directories(include)
 
# Source all *.cpp files from this project
file(GLOB microcv "src/*.cpp")

add_executable(microcv ${commsnode_sources})
target_link_libraries(microcv ${Boost_LIBRARIES})
