cmake_minimum_required (VERSION 3.0)

project(Game)


# find boost
find_package(Boost 1.59.0 REQUIRED)

include_directories("${Boost_INCLUDE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/src/ManaForgeEngine/include/")
include_directories("${CMAKE_SOURCE_DIR}/include/")

link_directories("${Boost_LIBRARY_DIRS}/clang 3.6.2")


add_executable(Game "src/RPG-Game.cpp")

add_dependencies(Game ManaForgeEngine)
target_link_libraries(Game ManaForgeEngine)
target_link_libraries(Game "stdc++")
target_link_libraries(Game "boost_system")
target_link_libraries(Game "boost_filesystem")

	
target_link_libraries(Game "dl")




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
