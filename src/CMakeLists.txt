cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${APUE_BINARY_DIR})

add_subdirectory(ch1)
add_subdirectory(ch3)
add_subdirectory(ch4)
add_subdirectory(ch5)
add_subdirectory(ch6)
add_subdirectory(ch9)
add_subdirectory(ch10)
add_subdirectory(ch13)
add_subdirectory(ch15)
add_subdirectory(ch16)
<<<<<<< HEAD
add_subdirectory(ch17)
add_subdirectory(ch18)
=======
add_subdirectory(ch18)
add_subdirectory(ch19)
>>>>>>> 4f0009c3e578794ddb7001cf1f36c95a825f99e1
add_subdirectory(other)

include_directories(${APUE_SOURCE_DIR}/include)

add_executable(apue apue.c)
target_link_libraries(apue
  dl
  utils
)
