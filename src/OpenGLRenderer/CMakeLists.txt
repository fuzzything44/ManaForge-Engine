
include(${CMAKE_SOURCE_DIR}/CMake/build.cmake)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)

add_subdirectory("dependencies/GLEW/build/cmake/")

add_subdirectory("dependencies/GLFW/")

add_subdirectory("dependencies/SOIL")


set(OPENGL_PRIVATE_FILES
	"src/OpenGLRendererManager.cpp;"
	"src/OpenGLRendererPCH.cpp;"
	"src/OpenGLRendererPCH.h;"
	)
	
set(OPENGL_PUBLIC_FILES
	"include/COpenGLModel.h;"
	"include/OpenGLMaterialInstance.h;"
	"include/OpenGLMaterialSource.h;"
	"include/OpenGLModelData.h;"
	"include/OpenGLRendererConfig.h;"
	"include/OpenGLRendererManager.h;"
	"include/OpenGLTexture.h;"
	)

buildmodule(OpenGLRenderer "${OPENGL_PRIVATE_FILES}${OPENGL_PUBLIC_FILES}")

add_dependencies(OpenGLRenderer ManaForgeEngine glew_s glfw SOIL)
target_link_libraries(OpenGLRenderer ManaForgeEngine glew_s glfw SOIL)

