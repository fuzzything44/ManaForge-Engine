
include(${CMAKE_SOURCE_DIR}/CMake/build.cmake)

set(CMAKE_AUTOMOC ON)

set(OPENGL_PRIVATE_FILES
	"src/OpenGLRendererManager.cpp;"
	"src/OpenGLRendererPCH.cpp;"
	"src/OpenGLModelData.cpp;"
	"src/OpenGLMaterialSource.cpp;"
	"src/OpenGLMaterialInstance.cpp;"
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

set(OpenGLRenderer_LINK_LIBRARIES "Box2D" CACHE INTERNAL "")
set(OpenGLRenderer_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include;" CACHE INTERNAL "")

buildmodule(OpenGLRenderer "${OPENGL_PRIVATE_FILES}${OPENGL_PUBLIC_FILES}" "ManaForgeEngine")
