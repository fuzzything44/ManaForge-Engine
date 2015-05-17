#pragma once

#include <Engine.h>

// link to required libs
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glew32.lib")

#define OpenGLRenderer_ModuleName "OpenGLRenderer"

#ifdef _MSC_VER
#	ifdef OpenGLRenderer_Source
#		define OpenGLRenderer_API __declspec(dllexport)
#	else
#		define OpenGLRenderer_API __declspec(dllimport)
#		pragma comment(lib, BOOST_PP_CAT(OpenGLRenderer_ModuleName, ".lib"))
#	endif
#endif

#include <GLTools/glew.h>
#include <GLTools/glfw3.h>