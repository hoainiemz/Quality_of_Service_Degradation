cmake_minimum_required(VERSION 3.28.3)
project(Quality_of_Service_Degradation)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Path to LibTorch (must point to libtorch/share/cmake/Torch)
set(CMAKE_PREFIX_PATH "$ENV{HOME}/libtorch")  # Adjust as needed

# Enable ccache if available
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS "Using ccache")
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

# Torch
find_package(Torch REQUIRED)
find_library(CUDA_CUDART_LIBRARY cudart HINTS /usr/local/cuda-12.6/lib64)


# Executable and sources
add_executable(Quality_of_Service_Degradation
        main.cpp
        Grapher.cpp
        Constants.cpp
        MyRandom.cpp
        IterativeSolution.cpp
        IterativeGreedy.cpp
        AdaptiveTrading.cpp
        SamplingApproach.cpp
        Utils.cpp
        LinearRounding.cpp
)

# Precompiled headers (CMake ≥ 3.16)
target_precompile_headers(Quality_of_Service_Degradation PRIVATE pch.h)

# Include directories if needed
target_include_directories(Quality_of_Service_Degradation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link Torch
#target_link_libraries(Quality_of_Service_Degradation "${TORCH_LIBRARIES}")
target_link_libraries(Quality_of_Service_Degradation
        "${TORCH_LIBRARIES}"
        ${CUDA_CUDART_LIBRARY}
)


# Use only if you really use CUDA (optional)
# find_package(CUDA REQUIRED)
# target_link_libraries(Quality_of_Service_Degradation ${CUDA_CUDART_LIBRARY})
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

