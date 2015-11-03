#pragma once

#pragma comment(lib, "GLFW3.lib")
#ifdef _DEBUG
#	pragma comment(lib, "glew32d.lib")
#else
#	pragma comment(lib, "glew32.lib")
#endif
#pragma comment(lib, "OpenGL32.lib")

#ifdef OpenGLRenderer_Source
#	define OpenGLRenderer_API __declspec(dllexport)
#else
#	define OpenGLRenderer_API __declspec(dllimport)
#	pragma comment(lib, "OpenGLRenderer.lib")
#endif
