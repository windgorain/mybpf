SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/out) 

aux_source_directory(./ SRC)
add_executable(mini ${SRC})
target_link_libraries(mini sutil dl pthread m elf)

