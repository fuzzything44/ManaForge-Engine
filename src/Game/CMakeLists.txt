
project(Game)


# find boost
find_package(Boost 1.59.0 REQUIRED filesystem system)

include_directories("${Boost_INCLUDE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/src/ManaForgeEngine/include/")
include_directories("${CMAKE_SOURCE_DIR}/include/")
include_directories("${CMAKE_SOURCE_DIR}/src/")


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/debug/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/debug/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/debug/")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/release/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/release/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/release/")

add_executable(Game "${CMAKE_CURRENT_SOURCE_DIR}/src/RPG-Game.cpp")

add_dependencies(Game ManaForgeEngine)
target_link_libraries(Game ManaForgeEngine)
if(${UNIX})
	target_link_libraries(Game "stdc++")
	target_link_libraries(Game "dl")
endif()
target_link_libraries(Game ${Boost_FILESYSTEM_LIBRARY})
target_link_libraries(Game ${Boost_SYSTEM_LIBRARY})

add_definitions("-DGame_Source")
add_definitions("-DBOOST_ALL_NO_LIB")




# will make sure we have a proper C++11 compiler
target_compile_features(Game PRIVATE 
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

add_dependencies(Game ManaForgeEngine TestContent)
target_link_libraries(Game ManaForgeEngine)
