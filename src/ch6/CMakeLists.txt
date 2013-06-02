cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${APUE_BINARY_DIR}/bin)

include_directories(${APUE_SOURCE_DIR}/include)

add_executable(ch6_0 ch6_0.c)

target_link_libraries(ch6_0 utils)
