cmake_minimum_required(VERSION 2.8)

project(GDProtoProfiling)

add_subdirectory(../../build  ${CMAKE_CURRENT_BINARY_DIR}/ProfilingLib)

include_directories(${GDPROTO_PROFILING_INCLUDE_DIRS})
link_directories(${GDPROTO_PROFILING_LIB_DIRS})

add_executable(PerfProfilerBasicExample
    main.cpp
)

target_link_libraries(PerfProfilerBasicExample
    ${GDPROTO_PROFILING_LIBS}
)
