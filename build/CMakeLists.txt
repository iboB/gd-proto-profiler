cmake_minimum_required(VERSION 2.8)

project(GDProtoProfiling)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/../src)
set(INC ${CMAKE_CURRENT_SOURCE_DIR}/../include/gdproto/profiling)

include_directories(${INC})

add_library(GDProtoProfiling STATIC
    ${INC}/Config.h
    ${INC}/Profile.h
    ${INC}/ProfilerManager.h
    ${SRC}/ProfilerManager.cpp
    ${INC}/PerfProfiler.h
    ${SRC}/PerfProfiler.cpp
    ${INC}/PerfProfilerNode.h
    ${SRC}/PerfProfilerNode.cpp
    ${INC}/PerfProfilerSection.h
    ${SRC}/PerfProfilerSection.cpp
    ${INC}/internal/high_res_clock.h
    ${SRC}/high_res_clock.cpp
)

set(GDPROTO_PROFILING_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../include
    CACHE STRING "Profiling include paths"
)

set(GDPROTO_PROFILING_LIB_DIRS ${CMAKE_CURRENT_BINARY_DIR}
    CACHE STRING "Profiling library paths"
)

set(GDPROTO_PROFILING_LIBS GDProtoProfiling
    CACHE STRING "Profiling libraries"
)
