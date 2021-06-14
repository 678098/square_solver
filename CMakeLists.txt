cmake_minimum_required(VERSION 3.5)
project(square_solver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_DEFAULT 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(CMAKE_BUILD_TYPE STREQUAL "")
	set(CMAKE_BUILD_TYPE Release)
endif()

option(WITH_BOOST_MULTIPRECISION "Allow boost multiprecision support for storing large float values" ON)

if(WITH_BOOST_MULTIPRECISION)
	add_definitions(-DWITH_BOOST_MULTIPRECISION)
endif()

set(SOURCES
    "src/args_parser.cpp"
	"src/main.cpp"
)

add_executable(square_solver ${SOURCES})

target_include_directories(square_solver
	PUBLIC
		"include/"
)

target_link_libraries(square_solver pthread)
