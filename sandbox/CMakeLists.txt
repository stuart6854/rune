cmake_minimum_required(VERSION 3.12)

project (sandbox)

set(SANDBOX_SRC_DIR "${SANDBOX_DIR}/src")
file(GLOB sandbox_src_files "${SANDBOX_SRC_DIR}/main.cpp")

# Create executable
add_executable(sandbox ${sandbox_src_files})
target_include_directories(sandbox PRIVATE ${SANDBOX_SRC_DIR})
target_compile_features(sandbox PRIVATE cxx_std_17)

# Link engine to client
target_link_libraries(sandbox rune)
target_include_directories(sandbox PRIVATE "${RUNE_DIR}/include")

add_custom_target(copy_assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets"  "${CMAKE_CURRENT_BINARY_DIR}/assets"
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/config"  "${CMAKE_CURRENT_BINARY_DIR}/config"
)
add_dependencies(sandbox copy_assets)