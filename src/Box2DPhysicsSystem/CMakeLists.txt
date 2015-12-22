
include(${CMAKE_SOURCE_DIR}/CMake/build.cmake)


# get the dependency
add_subdirectory("dependencies/Box2D")

set(BOX2D_PHYSICS_SYSTEM_PRIVATE_FILES
	"src/Box2DPhysicsManager.cpp;"
	)
	
set(BOX2D_PHYSICS_SYSTEM_PUBLIC_FILES
	"include/Box2DPhysicsManager.h;"
	"include/Box2DPhysicsSystemConfig.h;"
	"include/CBox2DCollision.h;"
	)


set(Box2DPhysicsSystem_LINK_LIBRARIES "Box2D" CACHE INTERNAL "")
set(Box2DPhysicsSystem_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include;${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Box2D;" CACHE INTERNAL "")


buildmodule(Box2DPhysicsSystem "${BOX2D_PHYSICS_SYSTEM_PRIVATE_FILES}${BOX2D_PHYSICS_SYSTEM_PUBLIC_FILES}" "ManaForgeEngine")


add_dependencies(Box2DPhysicsSystem Box2D)
