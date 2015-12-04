# cmake -DBoost_INCLUDE_DIR="/home/russellg/Documents/boost/boost_1_59_0/"
#  -DBoost_LIBRARY_DIR="/home/russellg/Documents/boost/boost_1_59_0/lib/clang 3.6.2"
#  -DCMAKE_C_COMPILER="clang" -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_BUILD_TYPE="DEBUG" ..

cmake_minimum_required (VERSION 3.0)


project(ManaForgeEngine)



if(${UNIX})
	SET(CMAKE_CXX_FLAGS "-stdlib=libc++ -O0 -UNDEBUG -DDEBUG -D_DEBUG -fsanitize=address")
    SET(CMAKE_C_FLAGS "-O0 -UNDEBUG -DDEBUG -D_DEBUG")
endif()

# find boost
find_package(Boost 1.59.0 REQUIRED filesystem system)

SET(MF_BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

# for now just compile the engine
add_subdirectory("src/ManaForgeEngine/")

add_subdirectory("src/Box2DPhysicsSystem/")

add_subdirectory("src/OpenALAudioSystem/")

add_subdirectory("src/OpenGLRenderer/")

add_subdirectory("src/TestContent/")

add_subdirectory("src/Game/")
