
include(${CMAKE_SOURCE_DIR}/CMake/build.cmake)

add_subdirectory("dependencies/GLEW/build/cmake/")

add_subdirectory("dependencies/GLFW/")

add_subdirectory("dependencies/SOIL")

if(${UNIX})
	SET_TARGET_PROPERTIES(glew_s PROPERTIES
		COMPILE_FLAGS "-fPIC"
	)

endif()


set(OPENGL_PRIVATE_FILES
	"src/OpenGLRendererManager.cpp;"
	"src/OpenGLRendererPCH.cpp;"
	"src/OpenGLModelData.cpp"
	"src/OpenGLMaterialSource.cpp"
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

