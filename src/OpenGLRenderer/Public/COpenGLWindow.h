#pragma once
#include <Engine.h>

#include <string>

#include <GLFW/glfw3.h>

struct COpenGLWindow
{

	COpenGLWindow(ivec2 size = { 100, 100 }, uvec2 pos = {0u, 0u}, const std::string& title = "")
	{
		if (glfwInit() == GL_FALSE)
		{
			MFLOG(Error) << "Failed to initalize GLFW.";
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
		assert(window);
	}

	~COpenGLWindow()
	{
		glfwDestroyWindow(window);
	}

	GLFWwindow* window;
};
