cmake_minimum_required(VERSION 3.14)
project(mineiguess)

#so that clang dont give you errors also so that so taht you can use debugger(so it sees paths)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# add your other .cpp files but use there relative path as you want your other .cpp files to be # # src so the path becomes /src/other.ccp
add_executable(
main
main.cpp
external/glad/src/glad.c
src/Shader.cpp
src/renderer.cpp
)
include(FetchContent)
if (NOT glfw3_FOUND)
  FetchContent_Declare(
         glfw
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG        3.4 # release-1.10.0
    )
  # the default glfw cmake is read by FetchContent_MakeAvailable and linked against glfw
  FetchContent_MakeAvailable(glfw)
  message(STATUS "is glfwpopulated:  ${glfw_POPULATED}")
endif()

#dont know but find_package wouldnt seem to work
#find_package(glm QUIET)

include(FetchContent)
if(NOT glm_FOUND)
  # cant be turned on even you do -DGLM_BUILD_TESTS=OFF either remove cache bool or turn it on the cmake .
  set(GLM_BUILD_TESTS OFF  CACHE BOOL "BUILD TEST TURNED OFF" FORCE)
  message(STATUS "test build OFF")
  message(STATUS "BUILD SHARED LIB OFF")
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "BUILD SHARED LIB OFF" FORCE)
  message(STATUS "download is slow my friend")
  FetchContent_Declare(
 glm
 GIT_REPOSITORY	https://github.com/g-truc/glm.git
 GIT_TAG 	1.0.3
)
  FetchContent_MakeAvailable(glm)
  message(STATUS "is glmpopulated:  ${glm_POPULATED}")
endif()
# glm is header only so you might think why do we need target_link_libraries so the answer is it was like this in the official build build , i guess it does what target_include_ditrectories would do
target_link_libraries( main PRIVATE glm::glm-header-only)

# to locate/link your own header inside include
target_include_directories( main PRIVATE src)
target_include_directories( main PRIVATE external)
# Link glfw to main
target_link_libraries(main
PRIVATE
glfw
)
# glfw_SOURCE_DIR it is tyde
# f i guess used by the glfws
# default cmake to point where headers live
target_include_directories( main PRIVATE "${glfw_SOURCE_DIR}/include")
# so that compiler can expand include"glad/glad.h" to include "external/glad/include/glad/glad.h"
target_include_directories(main PRIVATE external/glad/include)
