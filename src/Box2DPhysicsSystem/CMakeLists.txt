cmake_minimum_required(VERSION 3.0)


# get the dependency
add_subdirectory("include/")

set(BOX2D_PHYSICS_SYSTEM_PRIVATE_FILES
	"src/Box2DPhysicsManager.cpp"
	)
	
set(BOX2D_PHYSICS_SYSTEM_PUBLIC_FILES
	"include/Box2DPhysicsManager.h"
	"include/Box2DPhysicsSystemConfig.h"
	"include/CBox2DCollision.h"
	)
	
# find boost
find_package(Boost 1.59.0 REQUIRED)

include_directories(${MF_INCLUDE_DIR})
include_directories("${Boost_INCLUDE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(Box2DPhysicsSystem SHARED ${BOX2D_PHYSICS_SYSTEM_PRIVATE_FILES} ${BOX2D_PHYSICS_SYSTEM_PUBLIC_FILES})

# will make sure we have a proper C++11 compiler
target_compile_features(Box2DPhysicsSystem PRIVATE 
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

add_dependencies(Box2DPhysicsSystem ManaForgeEngine)
