#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class MainWindow
{
public:

	static GLint MainWindow::run(GLuint width, GLuint height, const GLchar* title, GLFWmonitor* monitor, GLFWwindow* share);

	static GLvoid bindDraw(GLvoid(*drawIn)(GLfloat));
	static GLvoid bindKeyboard(GLvoid(*keyboradIn)(GLFWwindow*, GLfloat));
	static GLvoid bindInit(GLint(*initIn)());
	static GLvoid bindExit(GLint(*exitIn)());

	static GLvoid bindResize(GLvoid(*resizeIn)(GLFWwindow*, int, int));

	static GLvoid bindScroll(GLvoid(*scrollIn)(GLFWwindow*, GLdouble, GLdouble));

	static GLFWwindow* window;

private:
	static GLvoid(*draw)(GLfloat);
	static GLvoid(*keyboard)(GLFWwindow* window, GLfloat);
	static GLint(*init)();
	static GLint(*exit)();
	
	static GLvoid(*scroll)(GLFWwindow*, GLdouble, GLdouble);
};