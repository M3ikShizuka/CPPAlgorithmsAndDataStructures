# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Fix cmake compile test "is not able to compile a simple test program."
# set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
# set(CMAKE_C_COMPILER_WORKS 1)
# set(CMAKE_CXX_COMPILER_WORKS 1)

## Verbose Ninja, Makefile, etc.
## or just use argument -v on build: cmake.exe --build ...mybuild/x64-win-debug-mingw --target all -j 6 -v
# set(CMAKE_VERBOSE_MAKEFILE ON)
# set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON")
# set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON" FORCE)

# Common
set(COMMON_PROJECT_CPP_STANDARD 20 ON CACHE BOOL "" FORCE)
set(COMMON_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/common" ON CACHE BOOL "" FORCE)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP ON
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Include header directories. (old method)
#include_directories(${INCLUDE_COMMON_DIR})

# Project
project ("DataStructuresAndAlgorithms")
# Enable tests.
enable_testing()
# Include sub-projects.
# Algorithms
## Sort
add_subdirectory ("src/Algorithms/Sort/BubbleSort")
add_subdirectory ("src/Algorithms/Sort/HeapSort")
add_subdirectory ("src/Algorithms/Sort/SelectionSort")
add_subdirectory ("src/Algorithms/Sort/InsertionSort")
add_subdirectory ("src/Algorithms/Sort/MergeSort")
add_subdirectory ("src/Algorithms/Sort/QuickSort")
# Data Structures
## Non-linear
### Basic
#### Trees
add_subdirectory ("src/DataStructures/Non-linear/Basic/Trees/BinarySearchTree")
### Complex
#### Trees
add_subdirectory ("src/DataStructures/Non-linear/Complex/Trees/Heap")
add_subdirectory ("src/DataStructures/Non-linear/Complex/Trees/RedBlackTreeLoop")
add_subdirectory ("src/DataStructures/Non-linear/Complex/Trees/AVLTree/AVLTreeRecursion")
add_subdirectory ("src/DataStructures/Non-linear/Complex/Trees/AVLTree/AVLTreeLoop")
