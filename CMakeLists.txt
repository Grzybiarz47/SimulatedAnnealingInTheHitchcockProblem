cmake_minimum_required(VERSION 3.0.0)
project(Hitchcock VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -pedantic -lm -std=c++17")
set(DIR ${CMAKE_SOURCE_DIR})
set(SRC ${DIR}/src)
set(INC ${DIR}/inc)
set(EXECUTABLE_OUTPUT_PATH ${DIR}/bin)

file(GLOB SOURCES ${SRC}/*.cpp)
file(GLOB HEADERS ${INC}/*.hpp)
file(GLOB BUILD ${DIR}/build/*)

include_directories(${INC})
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

add_custom_target(check valgrind ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME})
add_custom_target(run ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME})
add_custom_target(cleanall rm -r ${BUILD})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)