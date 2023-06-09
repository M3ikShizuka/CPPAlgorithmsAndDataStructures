﻿# CMakeList.txt : CMake project for Heap, include source and define
# project specific logic here.
#

project("AVLTree")

# Add source to this project's executable.
add_executable(
	${PROJECT_NAME}
	"avltree.test.cpp"
	"avltree.h"
)

# Include header directories. (new method)
target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_INCLUDE_DIR})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${COMMON_PROJECT_CPP_STANDARD})
endif()

# GoogleTest requires at least C++14
target_link_libraries(
	${PROJECT_NAME}
	GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

# # cppcheck
# find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
# if(CMAKE_CXX_CPPCHECK)
#     # Check CppCheck version
#     set(CPP_CHECK_CMD ${CMAKE_CXX_CPPCHECK} --version)
#     execute_process(COMMAND ${CPP_CHECK_CMD}
#       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#       RESULT_VARIABLE CPP_CHECK_RESULT
#       OUTPUT_VARIABLE CPP_CHECK_VERSION
#       ERROR_VARIABLE CPP_CHECK_ERROR)
    
#     # Check if version could be extracted
#     if(CPP_CHECK_RESULT EQUAL 0)
#       # Get number of CPU cores
#       include(ProcessorCount)
#       ProcessorCount(CPU_CORES)

#       # Append desired arguments to CppCheck
#       list(
#         APPEND CMAKE_CXX_CPPCHECK
#           # Use all the available CPU cores
#           "-j ${CPU_CORES}" 
          
#           #Skip tests files
#           #"-i */gtest/*"

#           # Only show found errors
#           "--quiet" 
          
#           # Desired warning level in CppCheck
#           "--enable=style"
          
#           # Optional: Specified C++ version
#           "--std=c++20"
           
#           # Optional: Specified platform
#           "--platform=win64"
          
#           # Optional: suppression file stored in same directory as the top level CMake script
#           "--suppressions-list=${CMAKE_SOURCE_DIR}/cppcheck_suppressions.txt"

#           # Optional: Use inline suppressions
#           "--inline-suppr"
          
#           # Run CppCheck from the working directory, as specified in the add_custom_target command below
#           "."
#         )

#         add_custom_target(ANALYZE_CPPCHECK DEPENDS ${PROJECT_NAME}
#             COMMAND ${CMAKE_CXX_CPPCHECK}
#             WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#             COMMENT "Static code analysis using ${CPP_CHECK_VERSION}"
#         )
#     endif()
# endif()