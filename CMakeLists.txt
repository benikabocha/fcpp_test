cmake_minimum_required(VERSION 3.0)

project(fcpp_test)

set(CMAKE_CXX_STANDARD 14)

file(GLOB FCPP_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/ext/fcpp/*.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/fcpp)

add_executable(sample01 sample01.cpp ${FCPP_SOURCE})
source_group(fcpp FILES ${FCPP_SOURCE})
add_custom_command(TARGET sample01 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/sample
    ${CMAKE_CURRENT_BINARY_DIR}/sample
)
