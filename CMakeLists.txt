cmake_minimum_required (VERSION 3.10)

set(GLOBAL CMAKE_CXX_STANDARD 20)
set(GLOBAL CMAKE_CXX_STANDARD_REQUIRED ON)

project ("sfml_chess")

# Include sub-projects.
add_subdirectory ("src")
