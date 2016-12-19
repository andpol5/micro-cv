cmake_minimum_required(VERSION 2.8.8)
set(PROJECT_NAME_STR microcv)
project(${PROJECT_NAME_STR} C CXX)

# Find and add libjpeg, libpng and libtiff
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

# Set C++11 flag and warnings 
if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11 -Wall -Wextra -Werror)
endif()

# Add jpeg and png linker flags and then boostlibs
set(MCV_LINK_LIBRARIES "-ljpeg -lpng -ltiff ${Boost_LIBRARIES}")

# Defensive C++ warning flags
if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11 -Wall -Wextra)
endif()

#--------------------------------------
# Main module lib (all source files except main.cpp)
#--------------------------------------
set(PROJECT_LIB_NAME ${PROJECT_NAME_STR}_lib)
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDES})
# Source the cpp files  
set(MICROCV_LIB_SOURCES 
    src/ImageProcessing.cpp
    src/FileIo.cpp    
    src/Mat.cpp
    )

add_library(${PROJECT_LIB_NAME} ${MICROCV_LIB_SOURCES})

#--------------------------------------
# Main binary (main.cpp + the library)
#--------------------------------------
set(MAIN_SOURCE src/main.cpp)
add_executable(${PROJECT_NAME_STR} ${MAIN_SOURCE})
target_link_libraries(${PROJECT_NAME_STR} ${PROJECT_LIB_NAME} ${MCV_LINK_LIBRARIES})
