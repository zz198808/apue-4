cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${APUE_BINARY_DIR}/bin)

include_directories(${APUE_SOURCE_DIR}/include)

add_executable(ch18_10 ch18_10.c)
add_executable(ch18_12 ch18_12.c)

target_link_libraries(ch18_10 utils)
target_link_libraries(ch18_12 utils)
