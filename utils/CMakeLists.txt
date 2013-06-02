cmake_minimum_required(VERSION 2.8)

set(LIBRARY_OUTPUT_PATH ${APUE_BINARY_DIR}/lib)

set(UTILS_SRC
  error_handling.c
  library_loader.c
)

include_directories(${APUE_SOURCE_DIR}/include)

add_library(utils STATIC ${UTILS_SRC})
