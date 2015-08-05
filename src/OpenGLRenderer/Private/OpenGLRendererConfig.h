#pragma once

#include <Engine.h>

#ifdef OpenGLRenderer_Source
#define OpenGLRenderer_API __declspec(dllexport)
#else
#define OpenGLRenderer_API __declspec(dllimport)
#pragma comment(lib, "OpenGLRenderer.lib")
#endif

#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#include <GLTools/glfw3.h>
