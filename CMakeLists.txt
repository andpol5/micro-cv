cmake_minimum_required(VERSION 2.8.8)
set(PROJECT_NAME_STR microcv)
project(${PROJECT_NAME_STR} C CXX)

# Find and add libjpeg, libpng and libtiff
find_package(Threads REQUIRED)
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
    add_definitions(-std=c++11 -Wall -Wextra -Werror -pthread)
endif()

# Add jpeg and png linker flags and then boostlibs
set(MCV_LINK_LIBRARIES "-ljpeg -lpng -ltiff -pthread ${Boost_LIBRARIES}")

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

#--------------------------------------
# Test (adapted from github.com/snikulov/google-test-examples/)
#--------------------------------------
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)
add_subdirectory(${EXT_PROJECTS_DIR}/gtest)

enable_testing()

set(PROJECT_TEST_NAME test_${PROJECT_NAME_STR})
include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES})

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
add_dependencies(${PROJECT_TEST_NAME} googletest)


if(NOT WIN32 OR MINGW)
    target_link_libraries(${PROJECT_TEST_NAME}
        ${GTEST_LIBS_DIR}/libgtest.a
        ${GTEST_LIBS_DIR}/libgtest_main.a
        )
else()
    target_link_libraries(${PROJECT_TEST_NAME}
        debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
        optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
        )
    target_link_libraries(${PROJECT_TEST_NAME}
        debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
        optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
        )
endif()

target_link_libraries(${PROJECT_TEST_NAME} ${PROJECT_LIB_NAME} ${MCV_LINK_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
add_test(test1 ${PROJECT_TEST_NAME})
