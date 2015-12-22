# cmake -DBoost_INCLUDE_DIR="/home/russellg/Documents/boost/boost_1_59_0/"
#  -DBoost_LIBRARY_DIR="/home/russellg/Documents/boost/boost_1_59_0/lib/clang 3.6.2"
#  -DCMAKE_C_COMPILER="clang" -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_BUILD_TYPE="DEBUG" ..

cmake_minimum_required (VERSION 3.0)


project(ManaForgeEngine)

set(CMAKE_AUTOMOC ON)

set(ManaForgeEngine_RESOURCE_DIR "\"${CMAKE_CURRENT_SOURCE_DIR}/Resource\"")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeConfig.in
	${CMAKE_BINARY_DIR}/Config.h)

include_directories(${CMAKE_BINARY_DIR})
	
if(${UNIX})
	SET(CMAKE_CXX_FLAGS "-stdlib=libc++ -fsanitize=address -lc++abi")
endif()

# find boost
find_package(Boost 1.59.0 REQUIRED filesystem system)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)

# for now just compile the engine
add_subdirectory("src/ManaForgeEngine/")

add_subdirectory("src/Box2DPhysicsSystem/")

add_subdirectory("src/OpenALAudioSystem/")

add_subdirectory("src/OpenGLRenderer/")

add_subdirectory("src/TestContent/")

add_subdirectory("src/Game/")
