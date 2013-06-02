cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${APUE_BINARY_DIR}/bin)

include_directories(${APUE_SOURCE_DIR}/include)

add_executable(ch1_1 ch1.1.c)
target_link_libraries(ch1_1 utils)
