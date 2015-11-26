cmake_minimum_required (VERSION 3.0)


project(ManaForgeEngine)

if(${UNIX})
	SET(CMAKE_CXX_FLAGS "-stdlib=libc++ -O0")
endif()

# find boost
find_package(Boost 1.59.0 REQUIRED filesystem system)


# for now just compile the engine
add_subdirectory("src/ManaForgeEngine/")

add_subdirectory("src/Box2DPhysicsSystem/")

add_subdirectory("src/OpenALAudioSystem/")

add_subdirectory("src/OpenGLRenderer/")

add_subdirectory("src/TestContent/")

add_subdirectory("src/Game/")
