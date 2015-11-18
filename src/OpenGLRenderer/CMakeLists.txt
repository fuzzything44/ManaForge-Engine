cmake_minimum_required(VERSION 3.0)

add_subdirectory("dependencies/GLEW/build/cmake/")

add_subdirectory("dependencies/GLFW/")

add_subdirectory("dependencies/SOIL")


project(OpenGLRenderer)


set(OPENGL_PRIVATE_FILES
	"src/OpenGLRendererManager.cpp"
	"src/OpenGLRendererPCH.cpp"
	"src/OpenGLRendererPCH.h"
	)
	
set(OPENGL_PUBLIC_FILES
	"include/COpenGLModel.h"
	"include/OpenGLMaterialInstance.h"
	"include/OpenGLMaterialSource.h"
	"include/OpenGLModelData.h"
	"include/OpenGLRendererConfig.h"
	"include/OpenGLRendererManager.h"
	"include/OpenGLTexture.h"
	)

# find boost
find_package(Boost 1.59.0 REQUIRED)	

include_directories("${CMAKE_SOURCE_DIR}/src/ManaForgeEngine/include/")
include_directories("${CMAKE_SOURCE_DIR}/src/")
include_directories("${Boost_INCLUDE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

message("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(OpenGLRenderer SHARED ${OPENGL_PRIVATE_FILES} ${OPENGL_PUBLIC_FILES})  

add_dependencies(OpenGLRenderer ManaForgeEngine glew_s glfw SOIL)

# will make sure we have a proper C++11 compiler
target_compile_features(OpenGLRenderer PRIVATE 
	cxx_constexpr 
	cxx_alias_templates
	cxx_decltype 
	cxx_lambdas
	cxx_lambda_init_captures
	cxx_nullptr
	cxx_override
	cxx_range_for
	cxx_right_angle_brackets
	cxx_static_assert
	cxx_strong_enums
	cxx_variadic_macros
	cxx_variadic_templates
	cxx_template_template_parameters
	
	 )


