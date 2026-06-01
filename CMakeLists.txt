cmake_minimum_required(VERSION 3.14)
project(mineiguess)

# Try to find a locally installed raylib, but don't quit on fail
find_package(GLFW QUIET)

# add your other .cpp files but use there relative path as you want your other .cpp files to be src so the path becomes /src/other.ccp
add_executable(
main
main.cpp
external/glad/src/glad.c
)
include(FetchContent)
if (NOT GLFW_FOUND)
    FetchContent_Declare(
         glfw
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG        3.4 # release-1.10.0
    )
    FetchContent_MakeAvailable(glfw)
endif()
# to locate/link your own header inside include
target_include_directories( main PRIVATE include)
# Link raylib to main
target_link_libraries(main
glfw 
)
target_include_directories( main PRIVATE "${glfw_SOURCE_DIR}/include")
target_include_directories(main PRIVATE external/glad/include)
