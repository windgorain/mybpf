SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/out/spf) 

set(SRC "")
aux_source_directory(./spfbuilder SRC)
add_executable(spfbuilder ${SRC})
target_link_libraries(spfbuilder sutil static_bs sutil static_bs dl pthread m)

set(SRC "")
aux_source_directory(./barebuilder SRC)
add_executable(barebuilder ${SRC})
target_link_libraries(barebuilder sutil static_bs sutil static_bs dl pthread m)

