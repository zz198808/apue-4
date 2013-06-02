cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${APUE_BINARY_DIR}/bin)

include_directories(${APUE_SOURCE_DIR}/include)

add_executable(ch3_2 ch3_2.c)
add_executable(ch3_3 ch3_3.c)
add_executable(ch3_4 ch3_4.c)

target_link_libraries(ch3_2 utils)
target_link_libraries(ch3_3 utils)
target_link_libraries(ch3_4 utils)
