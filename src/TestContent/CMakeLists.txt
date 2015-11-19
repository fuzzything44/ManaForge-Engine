
include(${CMAKE_SOURCE_DIR}/CMake/build.cmake)

project(TestContent)


set(TESTCONTENT_PRIVATE_FILES
	"src/TestContentSystem.cpp;"
	)
	
set(TESTCONTENT_PUBLIC_FILES
	"include/TestContentSystem.h;"
	"include/TestContentConfig.h;"
	)


include_directories("${CMAKE_SOURCE_DIR}/src/OpenALAudioSystem/include")
include_directories("${CMAKE_SOURCE_DIR}/src/Box2DPhysicsSystem/include")
include_directories("${CMAKE_SOURCE_DIR}/src/OpenGLRenderer/include")

buildmodule(TestContent "${TESTCONTENT_PRIVATE_FILES}${TESTCONTENT_PUBLIC_FILES}")  

add_dependencies(TestContent ManaForgeEngine OpenALAudioSystem OpenGLRenderer Box2DPhysicsSystem)
target_link_libraries(TestContent ManaForgeEngine OpenALAudioSystem OpenGLRenderer Box2DPhysicsSystem)

