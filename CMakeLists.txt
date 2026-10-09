cmake_minimum_required(VERSION 3.20)

project(
    ByteRoute
    VERSION 0.1.0
    DESCRIPTION "A user-space TCP/IP stack written in C"
    LANGUAGES C
)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(FATAL_ERROR "ByteRoute currently supports Linux only.")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/CompilerWarnings.cmake)

add_library(
    byteroute_core
    STATIC
    src/byteroute.c
    src/util/byteorder.c
)

add_library(ByteRoute::Core ALIAS byteroute_core)

target_include_directories(
    byteroute_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

byteroute_set_project_warnings(byteroute_core)

add_executable(
    byteroute
    src/main.c
)

target_link_libraries(
    byteroute
    PRIVATE
        ByteRoute::Core
)

byteroute_set_project_warnings(byteroute)

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()