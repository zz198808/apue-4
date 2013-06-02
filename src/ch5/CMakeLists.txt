cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${APUE_BINARY_DIR}/bin)

include_directories(${APUE_SOURCE_DIR}/include)

add_executable(ch5_3 ch5_3.c)
add_executable(ch5_4 ch5_4.c)
add_executable(ch5_5 ch5_5.c)

target_link_libraries(ch5_3 utils)
target_link_libraries(ch5_4 utils)
target_link_libraries(ch5_5 utils)
