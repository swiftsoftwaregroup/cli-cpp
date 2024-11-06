cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

project(cli-cpp)
set(target cli-cpp)

# First create a library with the shared code
add_library(${target}_lib)

# Operating System
string(TOLOWER ${CMAKE_SYSTEM_NAME} OS)

# debug definitions
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${target}_lib PUBLIC _DEBUG)
endif()

# release definitions
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(${target}_lib PUBLIC NDEBUG)
endif()

# macOS
if(OS STREQUAL "darwin") 
    # common compile options
    target_compile_options(${target}_lib PRIVATE -std=c++17 -stdlib=libc++)

    # x64 compile options
    target_compile_options(${target}_lib PRIVATE -m64 -fPIC)

    # debug compile options
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${target}_lib PRIVATE -g)
    endif()

    # release compile options
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(${target}_lib PRIVATE -Os)
    endif()
endif()

# sources
file(GLOB lib_source "src/*.cpp" "src/*.mm")
file(GLOB main_source "src/main.cpp") # Separate main.cpp
list(REMOVE_ITEM lib_source ${main_source}) # Remove main.cpp from lib sources

# Set up the library
target_sources(${target}_lib PRIVATE ${lib_source})

# Set up the main executable
add_executable(${target} ${main_source})
target_link_libraries(${target} PRIVATE ${target}_lib)

# third party libraries installed via vcpkg

## argparse
find_package(argparse CONFIG REQUIRED)
target_link_libraries(${target}_lib PUBLIC argparse::argparse)

# tests support
find_package(GTest CONFIG REQUIRED)
enable_testing()

add_executable(cli-cpp-test tests/test_main.cpp)
target_link_libraries(cli-cpp-test PRIVATE ${target}_lib GTest::gtest_main)

# Include directories
target_include_directories(${target}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

include(GoogleTest)
gtest_discover_tests(cli-cpp-test)

# docs
find_package(Doxygen REQUIRED)
find_program(SPHINX_EXECUTABLE sphinx-build REQUIRED)

# Doxygen configuration
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# Configure Doxyfile
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# Doxygen target
add_custom_command(
    OUTPUT ${DOXYGEN_INDEX_FILE}
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
    DEPENDS ${PROJECT_SOURCE_DIR}/src/greeting.hpp
    MAIN_DEPENDENCY ${DOXYFILE_OUT}
    COMMENT "Generating documentation with Doxygen"
    VERBATIM
)

# Sphinx target
set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/docs)
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/docs/sphinx)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

add_custom_target(docs
    COMMAND ${SPHINX_EXECUTABLE} -b html
    ${SPHINX_SOURCE} ${SPHINX_BUILD}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating documentation with Sphinx"
    VERBATIM
)