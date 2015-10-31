#pragma once

#pragma comment(lib, "GLFW3.lib")
#ifdef _DEBUG
#	pragma comment(lib, "glew32d.lib")
#else
#	pragma comment(lib, "glew32.lib")
#endif

#ifdef OpenGLRenderer_Source
#	define OpenGLRenderer_API __declspec(dllexport)
#else
#	define OpenGLRenderer_API __declspec(dllimport)
#endif
