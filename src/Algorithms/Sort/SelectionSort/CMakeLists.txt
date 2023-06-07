# CMakeList.txt : CMake project for Heap, include source and define
# project specific logic here.
#

project("SelectionSort")

# Add source to this project's executable.
add_executable(
	${PROJECT_NAME}
	"selectionsort.test.cpp"
	"selectionsort.h"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${COMMON_PROJECT_CPP_STANDARD})
endif()

# Include header directories. (new method)
target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_INCLUDE_DIR})

# GoogleTest requires at least C++14
target_link_libraries(
	${PROJECT_NAME}
	GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})