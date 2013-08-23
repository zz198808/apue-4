cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${APUE_BINARY_DIR}/bin)

include_directories(${APUE_SOURCE_DIR}/include)

add_executable(ch16_1 ch16_1.c)
add_executable(ch16_4 ch16_4.c)
add_executable(ch16_5 ch16_5.c)

add_executable(ruptime ruptime.c)
add_executable(ruptimed ruptimed.c)

target_link_libraries(ch16_1 utils)
target_link_libraries(ch16_4 utils)
target_link_libraries(ch16_5 utils)
target_link_libraries(ruptime utils)
target_link_libraries(ruptimed utils)
