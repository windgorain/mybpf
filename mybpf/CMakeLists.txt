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

# tools
add_subdirectory(./src/tool)

# spf_runtime
add_subdirectory(./src/spf_runtime)

