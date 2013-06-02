cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${APUE_BINARY_DIR})

add_subdirectory(ch1)
add_subdirectory(ch3)
add_subdirectory(ch4)
add_subdirectory(ch5)
add_subdirectory(ch6)
add_subdirectory(ch9)
add_subdirectory(ch10)
add_subdirectory(other)
  
include_directories(${APUE_SOURCE_DIR}/include)

add_executable(apue apue.c)
target_link_libraries(apue
  dl
  utils
)
