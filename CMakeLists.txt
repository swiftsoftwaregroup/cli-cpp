cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE "$ENV${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

project(cli-cpp)
set(target cli-cpp)

add_executable(${target})

# Operating System
string(TOLOWER ${CMAKE_SYSTEM_NAME} OS)

# output
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# debug definitions
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${target} PUBLIC _DEBUG)
endif()

# release definitions
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(${target} PUBLIC NDEBUG)
endif()

# macOS
if(OS STREQUAL "darwin") 
    # common compile options
    target_compile_options(${target} PRIVATE -std=c++20 -stdlib=libc++)

    # x64 compile options
    target_compile_options(${target} PRIVATE -m64 -fPIC)

    # debug compile options
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${target} PRIVATE -g)
    endif()

    # release compile options
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(${target} PRIVATE -Os)
    endif()
endif()

# sources
file(GLOB source "src/*.cpp" "src/*.mm")

target_sources(${target}
PRIVATE
    ${source} 
)

# third party libraries installed via vcpkg

## argparse
find_package(argparse CONFIG REQUIRED)
target_link_libraries(${target} PRIVATE argparse::argparse)
