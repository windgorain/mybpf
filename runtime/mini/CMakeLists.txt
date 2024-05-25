IF (UNIX AND NOT APPLE)
cmake_minimum_required(VERSION 2.8)
ENDIF()
IF (APPLE OR WIN32)
cmake_minimum_required(VERSION 3.2)
ENDIF()

INCLUDE(${CMAKE_SOURCE_DIR}/my_cmake_def.txt)

project (mybpf_runtime)

include_directories(./h)

# utl
add_subdirectory(./src/lib/util)

# example
add_subdirectory(./main)

