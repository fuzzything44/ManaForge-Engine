cmake_minimum_required (VERSION 3.0)


project(ManaForgeEngine)

set(CMAKE_AUTOMOC ON)

set(ManaForgeEngine_RESOURCE_DIR "\"${CMAKE_CURRENT_SOURCE_DIR}/Resource\"")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeConfig.in
	${CMAKE_BINARY_DIR}/Config.h)

include_directories(${CMAKE_BINARY_DIR})
include_directories(/home/russellg/hana-master/include/)
	
if(${UNIX})
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

# find boost
find_package(Boost 1.59.0 REQUIRED filesystem system)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Hana REQUIRED)

include_directories(${Hana_INCLUDE_DIR})

add_subdirectory("src/ManaForgeEngine/")

add_subdirectory("src/Box2DPhysicsSystem/")

add_subdirectory("src/OpenALAudioSystem/")

add_subdirectory("src/OpenGLRenderer/")

add_subdirectory("src/TestContent/")

add_subdirectory("src/Game/")
