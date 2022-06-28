cmake_minimum_required(VERSION 3.12)

project (rune)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set platform variables for path resolution
include(cmake/PlatformDetection.cmake)
set_platform_detection_vars()

# Setup third-party library dirs
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(FMT_DIR "${LIB_DIR}/fmt")
set(GLFW_DIR "${LIB_DIR}/glfw")
set(GLAD_DIR "${LIB_DIR}/glad")
set(GLM_DIR "${LIB_DIR}/glm")
set(SPDLOG_DIR "${LIB_DIR}/spdlog")
set(TOML_DIR "${LIB_DIR}/tomlplusplus")

message("${SPDLOG_DIR}")

# Source files
set(source_dir "${PROJECT_SOURCE_DIR}/src/")
file(GLOB 
	source_files
	"${source_dir}/init.cpp"
	"${source_dir}/systems/log.cpp"
	"${source_dir}/systems/config.cpp"
	"${source_dir}/systems/window.cpp"
	"${source_dir}/systems/graphics/graphics.cpp"
	"${source_dir}/systems/events/events.cpp"
	"${source_dir}/maths/random.cpp"
	"${source_dir}/utility/guid.cpp"
	"${source_dir}/platform/${target_platform}/sample.cpp"
	"${source_dir}/platform/${target_platform}/graphics.cpp"
	"${source_dir}/platform/${target_platform}/renderer_opengl.cpp"
	"${GLAD_DIR}/src/glad.c"
)

# Include dir
set(include_dir "${PROJECT_SOURCE_DIR}/include")

# Check for OpenGL
find_package(OpenGL)

if(OPENGL_FOUND)
	message("OpenGL found!")
	include_directories(${OPENGL_INCLUDE_DIR})
else()
	message("OpenGL not found!")
endif()

# Disable unwanted GLFW features
set(GLFW_BUILD_EXAMPLE OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate the installation target")

# spdlog features
set(SPDLOG_FMT_EXTERNAL OFF CACHE INTERNAL "Use external fmt library instead of bundled")

# Ser PIC for using fmt as shared lib
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

# Add subdirectories for third-party libs
add_subdirectory(${FMT_DIR})
add_subdirectory(${GLFW_DIR})
add_subdirectory(${GLM_DIR})
add_subdirectory(${SPDLOG_DIR})
add_subdirectory(${TOML_DIR})

# Define library
add_library(rune)
target_sources(rune PRIVATE ${source_files})
target_include_directories(rune PRIVATE ${include_dir})

# Configure pre-compiled headers
target_precompile_headers(rune INTERFACE "${PROJECT_SOURCE_DIR}/include/rune/pch.hpp")

# Link fmt
target_link_libraries(rune PUBLIC fmt)
target_include_directories(rune PUBLIC "${FMT_DIR}/include")

# Link glfw
target_link_libraries(rune PUBLIC glfw)
target_include_directories(rune PUBLIC "${GLFW_DIR}/include")
target_compile_definitions(rune PUBLIC "GLFW_INCLUDE_NONE")

# Link glad
target_include_directories(rune PUBLIC "${GLAD_DIR}/include")

# Link glm
target_link_libraries(rune PUBLIC glm)
target_include_directories(rune PUBLIC "${GLM_DIR}/include")

# Link spdlog
target_link_libraries(rune PUBLIC spdlog::spdlog)
target_include_directories(rune PUBLIC "${SPDLOG_DIR}/include")

# Link toml++
target_link_libraries(rune PUBLIC tomlplusplus::tomlplusplus)
target_include_directories(rune PUBLIC "${TOML_DIR}/include")