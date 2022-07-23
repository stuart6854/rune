add_library(mono SHARED IMPORTED GLOBAL)
set_target_properties(mono PROPERTIES
	IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/bin/Release/mono-2.0-sgen.dll"
	IMPORTED_LOCATION_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug/mono-2.0-sgen.dll"
	IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/Release/mono-2.0-sgen.lib"
	IMPORTED_IMPLIB_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/lib/Debug/mono-2.0-sgen.lib"
)
target_include_directories(mono INTERFACE "include")