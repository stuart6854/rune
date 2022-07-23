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
set(STB_DIR "${LIB_DIR}/stb")
set(ASSIMP_DIR "${LIB_DIR}/assimp")
set(SPIRV_REFLECT_DIR "${LIB_DIR}/spirv_reflect")

message("${SPDLOG_DIR}")

# Source files
set(source_dir "${PROJECT_SOURCE_DIR}/src/")
file(GLOB 
	source_files
	"${source_dir}/init.cpp"
	"${source_dir}/core/log.cpp"
	"${source_dir}/core/config.cpp"
	"${source_dir}/core/window.cpp"
	"${source_dir}/scripting/script_engine.cpp"
	"${source_dir}/graphics/renderer.cpp"
	"${source_dir}/graphics/graphics.cpp"
	"${source_dir}/graphics/texture.cpp"
	"${source_dir}/graphics/mesh.cpp"
	"${source_dir}/graphics/shader.cpp"
	"${source_dir}/graphics/material.cpp"
	"${source_dir}/events/events.cpp"
	"${source_dir}/assets/asset.cpp"
	"${source_dir}/assets/asset_factory.cpp"
	"${source_dir}/assets/asset_registry.cpp"
	"${source_dir}/input/input.cpp"
	"${source_dir}/maths/random.cpp"
	"${source_dir}/utility/guid.cpp"
	"${source_dir}/utility/buffer.cpp"
	"${source_dir}/platform/${target_platform}/sample.cpp"
	"${source_dir}/platform/${target_platform}/time.cpp"
	"${source_dir}/platform/${target_platform}/graphics.cpp"
	"${source_dir}/platform/opengl/renderer.cpp"
	"${source_dir}/platform/opengl/shader_reflection.cpp"
	"${GLAD_DIR}/src/glad.c"
)

# Include dir
set(include_dir "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/src")

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

# assimp features
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Build package with shared libraries.")
set(ASSIMP_NO_EXPORT OFF CACHE INTERNAL "Disable Assimp's export functionality.")
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "If the test suite for Assimp is built in addition to the library.")
set(ASSIMP_INSTALL OFF CACHE INTERNAL "Disable this if you want to use assimp as a submodule.")
set(ASSIMP_BUILD_ASSIMP_VIEW OFF CACHE INTERNAL "If the Assimp view tool is built. (requires DirectX)")
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE INTERNAL "default value of all ASSIMP_BUILD_XXX_EXPORTER values")
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE INTERNAL "default value of all ASSIMP_BUILD_XXX_IMPORTER values")
set(ASSIMP_BUILD_OBJ_IMPORTER ON)
set(ASSIMP_BUILD_FBX_IMPORTER ON)
set(ASSIMP_BUILD_GLTF_IMPORTER ON)
set(ASSIMP_BUILD_ASSBIN_EXPORTER ON)
set(ASSIMP_BUILD_ASSXML_EXPORTER ON)

# Ser PIC for using fmt as shared lib
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

# Add subdirectories for third-party libs
add_subdirectory(${FMT_DIR})
add_subdirectory(${GLFW_DIR})
add_subdirectory(${GLM_DIR})
add_subdirectory(${SPDLOG_DIR})
add_subdirectory(${TOML_DIR})
add_subdirectory(${ASSIMP_DIR})
add_subdirectory(${SPIRV_REFLECT_DIR})

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
target_compile_definitions(rune PUBLIC "GLM_FORCE_RADIANS" "GLM_FORCE_LEFT_HANDED")

# Link spdlog
target_link_libraries(rune PUBLIC spdlog::spdlog)
target_include_directories(rune PUBLIC "${SPDLOG_DIR}/include")

# Link toml++
target_link_libraries(rune PUBLIC tomlplusplus::tomlplusplus)
target_include_directories(rune PUBLIC "${TOML_DIR}/include")

# Link stb
target_include_directories(rune PUBLIC "${STB_DIR}/include")

# Link assimp
target_link_libraries(rune PUBLIC assimp::assimp)
target_include_directories(rune PUBLIC "${ASSIMP_DIR}/include")

# Link SPIRV_Reflect
target_link_libraries(rune PUBLIC spirv_reflect)
target_include_directories(rune PUBLIC "${SPIRV_REFLECT_DIR}")

# Link Mono
add_subdirectory(lib/mono)
target_link_libraries(rune PUBLIC mono)