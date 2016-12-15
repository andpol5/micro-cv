cmake_minimum_required(VERSION 2.8.8)
set(PROJECT_NAME_STR microcv)
project(${PROJECT_NAME_STR} C CXX)

# Find libjpeg, libpng and libtiff
find_package(JPEG REQUIRED)
if (JPEG_FOUND)
  	include_directories(${JPEG_INCLUDE_DIR})
	link_directories(${JPEG_LIBRARY_DIR})
endif()

find_package(PNG REQUIRED)
if (PNG_FOUND)
  	include_directories(${PNG_INCLUDE_DIR})
	link_directories(${PNG_LIBRARY_DIR})
endif()

find_package(TIFF REQUIRED)
if (TIFF_FOUND)
  	include_directories(${TIFF_INCLUDE_DIR})
	link_directories(${TIFF_LIBRARY_DIR})
endif()

# Try to find the Boost libraries and includes automagically
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS filesystem system REQUIRED)
if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIR})
	link_directories(${Boost_LIBRARY_DIR})
endif()

# Set C++11 flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Add jpeg and png linker flags and then boostlibs
set(MCV_LINK_LIBRARIES "-ljpeg -lpng -ltiff ${Boost_LIBRARIES}")

# Defensive C++ warning flags
if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11 -Wall -Wextra -Werror -ljpeg)
endif()

# Header files from this project
#include_directories(include)

#--------------------------------------
# Main binary (main.cpp + the library)
#--------------------------------------
set(MAIN_SOURCE src/main.cpp)
add_executable(${PROJECT_NAME_STR} ${MAIN_SOURCE})
target_link_libraries(${PROJECT_NAME_STR} ${MCV_LINK_LIBRARIES})
