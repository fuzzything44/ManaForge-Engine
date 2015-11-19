
include(${CMAKE_SOURCE_DIR}/CMake/build.cmake)


# get the dependency
add_subdirectory("include/")

set(BOX2D_PHYSICS_SYSTEM_PRIVATE_FILES
	"src/Box2DPhysicsManager.cpp;"
	)
	
set(BOX2D_PHYSICS_SYSTEM_PUBLIC_FILES
	"include/Box2DPhysicsManager.h;"
	"include/Box2DPhysicsSystemConfig.h;"
	"include/CBox2DCollision.h;"
	)

buildmodule(Box2DPhysicsSystem "${BOX2D_PHYSICS_SYSTEM_PRIVATE_FILES}${BOX2D_PHYSICS_SYSTEM_PUBLIC_FILES}")


add_dependencies(Box2DPhysicsSystem ManaForgeEngine Box2D)
target_link_libraries(Box2DPhysicsSystem ManaForgeEngine Box2D)
