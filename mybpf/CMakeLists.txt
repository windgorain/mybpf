IF (UNIX AND NOT APPLE)
cmake_minimum_required(VERSION 2.8)
ENDIF()
IF (APPLE OR WIN32)
cmake_minimum_required(VERSION 3.2)
ENDIF()

INCLUDE(${CMAKE_SOURCE_DIR}/my_cmake_def.txt)

project (mybpf)

include_directories(./h)

# utl
add_subdirectory(./src/lib/util)

# base
add_subdirectory(./src/lib/base)

# spf/runtime
add_subdirectory(./spf/runtime)

# spf/tools
add_subdirectory(./spf/tools)
