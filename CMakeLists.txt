cmake_minimum_required (VERSION 3.0)


project(ManaForgeEngine)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR})

# for now just compile the engine
add_subdirectory("src/ManaForgeEngine/")

add_subdirectory("src/Box2DPhysicsSystem/")

add_subdirectory("src/OpenALAudioSystem/")

add_subdirectory("src/OpenGLRenderer/")

add_subdirectory("src/TestContent/")

add_subdirectory("src/Game/")
