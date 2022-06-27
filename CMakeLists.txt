# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.12)

project (rune_proj LANGUAGES CXX)

set(SANDBOX_DIR "${PROJECT_SOURCE_DIR}/sandbox")
set(RUNE_DIR "${PROJECT_SOURCE_DIR}/rune")

# Include sub-projects.
add_subdirectory("sandbox")
add_subdirectory ("rune")
