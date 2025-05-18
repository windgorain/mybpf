SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/out/spf) 

set(SRC "")
aux_source_directory(./bare_cmd SRC)
add_executable(bare-cmd ${SRC})
target_link_libraries(bare-cmd sutil static_bs sutil static_bs dl pthread)

set(SRC "")
aux_source_directory(./bare_interactive SRC)
add_executable(bare-interactive ${SRC})
target_link_libraries(bare-interactive sutil static_bs sutil static_bs dl pthread)

set(SRC "")
aux_source_directory(./spf_cmd SRC)
add_executable(spfcmd ${SRC})
target_link_libraries(spfcmd sutil static_bs sutil static_bs dl pthread)

