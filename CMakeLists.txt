cmake_minimum_required(VERSION 3.14)
project(mineiguess)

# Try to find a locally installed raylib, but don't quit on fail
find_package(raylib 5.5 QUIET)

# add your other .cpp files but use there relative path as you want your other .cpp files to be src so the path becomes /src/other.ccp
add_executable(
main
main.cpp
../src/perlin.cpp
)

# This code downloads raylib into a directory called _deps and adds it as a subdirectory, compiling it with the program when running the build command
include(FetchContent)
if (NOT raylib_FOUND)
    FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP True #This option is not required but suppresses a warning
    )
    FetchContent_MakeAvailable(raylib)
endif()

# We don't want raylib's examples built. This option is picked up by raylib's CMakeLists.txt
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# to locate/link your own header inside include
target_include_directories( main PRIVATE include)

# Link raylib to main
target_link_libraries(main
    raylib
)

# Make main find the <raylib.h> header (and others)
target_include_directories(main PUBLIC "${raylib_SOURCE_DIR}/src")