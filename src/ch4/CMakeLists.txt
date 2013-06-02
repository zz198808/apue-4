cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${APUE_BINARY_DIR}/bin)

include_directories(${APUE_SOURCE_DIR}/include)

add_executable(ch4_1 ch4_1.c)
add_executable(ch4_2 ch4_2.c)
add_executable(ch4_3 ch4_3.c)
add_executable(ch4_4 ch4_4.c)
add_executable(ch4_5 ch4_5.c)
add_executable(ch4_6 ch4_6.c)
add_executable(ch4_8 ch4_8.c)

target_link_libraries(ch4_1 utils)
target_link_libraries(ch4_2 utils)
target_link_libraries(ch4_3 utils)
target_link_libraries(ch4_4 utils)
target_link_libraries(ch4_5 utils)
target_link_libraries(ch4_6 utils)
target_link_libraries(ch4_8 utils)