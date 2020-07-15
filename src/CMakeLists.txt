aux_source_directory(. SRC_DIR)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

include_directories(/usr/local/include)

add_executable(dii_stt ${SRC_DIR})

find_library(GCTL_LIBRARY gctl /usr/local/lib)

target_link_libraries(dii_stt PUBLIC ${GCTL_LIBRARY})

find_library(LBFGS_LIBRARY lbfgs /usr/local/lib)

target_link_libraries(dii_stt PUBLIC ${LBFGS_LIBRARY})