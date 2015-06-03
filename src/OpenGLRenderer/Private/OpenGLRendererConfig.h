#pragma once

#include <Engine.h>

// link to required libs
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "OpenGL32.lib")

#ifdef _DEBUG
#	pragma comment(lib, "glew32d.lib")
#else 
#	pragma comment(lib, "glew32.lib")
#endif

#define OpenGLRenderer_ModuleName "OpenGLRenderer"

#ifdef OpenGLRenderer_Source
#	define OpenGLRenderer_API __declspec(dllexport)
#else
#	define OpenGLRenderer_API __declspec(dllimport)
#	pragma comment(lib, BOOST_PP_CAT("modules\\", BOOST_PP_CAT(OpenGLRenderer_ModuleName, ".lib")))
#endif
#

#include <GL/glew.h>

#include <GLTools/glfw3.h>