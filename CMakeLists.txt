# Define the minimum required version for this CMake project.
cmake_minimum_required(VERSION 3.20)

# Project name
project("MinLib" LANGUAGES CXX)

# This is for LSPs.
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# Because I love C++20.
set(CMAKE_CXX_STANDARD 20)

# Copy our beloved compile_commands.json for LSPs.
# configure_file("${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json"
# "${CMAKE_CURRENT_SOURCE_DIR}" COPYONLY)

# Recursively fetch all source files for compilation. NOTE: In actuality you do
# not need to pass header files (such as .h and .hpp) to the compiler for
# compilation because they do not compile, but the reason why I am doing this is
# because when for example, generating a Visual Studio Solution (by passing
# Visual Studio 17 as the generator argument), Visual Studio will not place the
# header files inside its filters meaning you will only see the source files so
# it is a good practice to pass the headers as well.
file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE HDR_FILES "src/*.h")
file(GLOB_RECURSE HPP_FILES "src/*.hpp")

# Create a custom build command and a dummy target for it to copy
# compile_commands.json to the source directory for LSPs.
add_custom_command(
  OUTPUT ${CMAKE_SOURCE_DIR}/compile_commands.json
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/compile_commands.json
          ${CMAKE_SOURCE_DIR}/compile_commands.json
  DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
  COMMENT "Copying compile_commands.json...")

add_custom_target(copy_compile_commands ALL
                  DEPENDS ${CMAKE_SOURCE_DIR}/compile_commands.json)

# Recursively fetch all the subdirectories inside src/ so that I can make them
# public.
file(GLOB SRC_SUB_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/*")

# Create our library target.
add_library(MinLib ${SRC_FILES} ${HDR_FILES} ${HPP_FILES})

# Not an actual test. add_executable(MinLib-Test ${SRC_FILES} ${HDR_FILES}
# ${HPP_FILES})

# Make src/ public to include header from.
target_include_directories(MinLib PUBLIC src/)

foreach(SUBDIR ${SRC_SUB_DIRS})
  if(IS_DIRECTORY ${SUBDIR})
    target_include_directories(MinLib PUBLIC ${SUBDIR})
  endif()
endforeach()
