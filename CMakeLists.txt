# ================================================
#  AQC for 2-SAT problem
#  Author: Yiming Ding, Westlake Univ
#  Last updated: Jan.9, 2024
# ================================================
cmake_minimum_required(VERSION 3.10)
project(2_sat)
set(CMAKE_CXX_STANDARD 17)
set(USER_SOURCE "./main.cpp")
set(OUTPUT_EXE "run")

if (NOT DEFINED ${QuEST_LIB_PATH})
    # Build libQuEST.so
    set(QuEST_DIR "QuEST" CACHE STRING
            "Name of the directory containing the QuEST library sources. It must be located in the same directory as the root CMakeLists.txt")
    add_subdirectory(${QuEST_DIR})
    set(QuEST_LIB_PATH "${CMAKE_CURRENT_BINARY_DIR}/${QuEST_DIR}")
    set(QuEST_LIB_EXACT "${QuEST_LIB_PATH}/libQuEST.so")
endif()

add_executable(${OUTPUT_EXE} ${USER_SOURCE})

if (WIN32)
    target_link_libraries(${OUTPUT_EXE} QuEST)
else ()
    target_link_libraries(${OUTPUT_EXE} QuEST m)
endif()