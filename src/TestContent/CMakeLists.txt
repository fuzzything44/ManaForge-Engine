cmake_minimum_required (VERSION 3.0)

project(TestContent)


set(TESTCONTENT_PRIVATE_FILES
	"src/TestContentSystem.cpp"
	)
	
set(TESTCONTENT_PUBLIC_FILES
	"include/TestContentSystem.h"
	"include/TestContentConfig.h"
	)

# find boost
find_package(Boost 1.59.0 REQUIRED)	

include_directories("${CMAKE_SOURCE_DIR}/src/ManaForgeEngine/include/")
include_directories("${CMAKE_SOURCE_DIR}/src/")
include_directories("${Boost_INCLUDE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

include_directories("${CMAKE_SOURCE_DIR}/src/OpenALAudioSystem/include")
include_directories("${CMAKE_SOURCE_DIR}/src/Box2DPhysicsSystem/include")
include_directories("${CMAKE_SOURCE_DIR}/src/OpenGLRenderer/include")

add_library(TestContent SHARED ${TESTCONTENT_PRIVATE_FILES} ${TESTCONTENT_PUBLIC_FILES})  

add_dependencies(TestContent OpenALAudioSystem OpenGLRenderer Box2DPhysicsSystem ManaForgeEngine)

# will make sure we have a proper C++11 compiler
target_compile_features(TestContent PRIVATE 
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

