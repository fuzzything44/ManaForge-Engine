
include(${CMAKE_SOURCE_DIR}/CMake/build.cmake)

project(TestContent)


set(TESTCONTENT_PRIVATE_FILES
	"src/TestContentSystem.cpp;"
	)
	
set(TESTCONTENT_PUBLIC_FILES
	"include/TestContentSystem.h;"
	"include/TestContentConfig.h;"
	)


set(TestContent_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include;" CACHE INTERNAL "")


buildmodule(TestContent "${TESTCONTENT_PRIVATE_FILES};${TESTCONTENT_PUBLIC_FILES}" 
	"ManaForgeEngine;OpenALAudioSystem;OpenGLRenderer;Box2DPhysicsSystem")  


