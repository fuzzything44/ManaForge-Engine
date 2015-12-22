
project(Game)


# find boost
find_package(Boost 1.59.0 REQUIRED filesystem system)

include_directories("${Boost_INCLUDE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/src/ManaForgeEngine/include/")
include_directories("${CMAKE_SOURCE_DIR}/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")
include_directories("${CMAKE_SOURCE_DIR}/src/")

add_executable(Game "src/RPG-Game.cpp")

if(${UNIX})
	target_link_libraries(Game "dl")
endif()

add_dependencies(Game ManaForgeEngine OpenALAudioSystem OpenGLRenderer TestContent Box2DPhysicsSystem)

target_link_libraries(Game ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ManaForgeEngine)

set_target_properties(Game
	PROPERTIES

	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"

)

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
		
target_link_libraries(Game Qt5::Widgets Qt5::Gui) 

