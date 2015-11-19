

macro(buildmodule MODULE_NAME MODULE_FILES)

	project(${MODULE_NAME})


	# find boost
	find_package(Boost 1.59.0 REQUIRED filesystem system)

	include_directories("${Boost_INCLUDE_DIR}")
	include_directories("${CMAKE_SOURCE_DIR}/src/ManaForgeEngine/include/")
	include_directories("${CMAKE_SOURCE_DIR}/include/")
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")
	include_directories("${CMAKE_SOURCE_DIR}/src/")


	add_library(${MODULE_NAME} SHARED ${MODULE_FILES})

	if(LINUX)
		target_link_libraries(${MODULE_NAME} "stdc++")
		target_link_libraries(${MODULE_NAME} "dl")
	endif()
	target_link_libraries(${MODULE_NAME} ${Boost_FILESYSTEM_LIBRARY})
	target_link_libraries(${MODULE_NAME} ${Boost_SYSTEM_LIBRARY})

	add_definitions("-D${MODULE_NAME}_Source")
	add_definitions("-DBOOST_ALL_NO_LIB")
	
	set_target_properties( ${MODULE_NAME}
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/debug/"
		LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/debug/"
		RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/debug/"
		
		
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/release/"
		LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/release/"
		RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/release/"
	)
	
	# will make sure we have a proper C++11 compiler
	target_compile_features(${MODULE_NAME} PRIVATE 
		cxx_constexpr 
		cxx_alias_templates
		cxx_decltype 
		cxx_lambdas
		cxx_lambda_init_captures
		cxx_nullptr
		cxx_override
		cxx_range_for
		cxx_right_angle_brackets
		cxx_static_assert
		cxx_strong_enums
		cxx_variadic_macros
		cxx_variadic_templates
		cxx_template_template_parameters
		
		 )


endmacro(buildmodule)


