cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${APUE_BINARY_DIR}/bin)

include_directories(${APUE_SOURCE_DIR}/include)

add_executable(add2 add2.c)
add_executable(add2_sys add2_sys.c)
add_executable(ch15_9 ch15_9.c)

target_link_libraries(add2 utils)
target_link_libraries(add2_sys utils)
target_link_libraries(ch15_9 utils)

