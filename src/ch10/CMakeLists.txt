cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${APUE_BINARY_DIR}/bin)

include_directories(${APUE_SOURCE_DIR}/include)

add_executable(ch10_6 ch10_6.c)
add_executable(ch10_7 ch10_7.c)

target_link_libraries(ch10_6 utils)
target_link_libraries(ch10_7 utils)
