
INCLUDE(${CMAKE_SOURCE_DIR}/my_cmake_def.txt)

set(SRC "")

aux_source_directory(./mybpf SRC)
aux_source_directory(./bpf_helper SRC)

add_library(sutil ${SRC})
set_property(TARGET sutil PROPERTY POSITION_INDEPENDENT_CODE ON)

