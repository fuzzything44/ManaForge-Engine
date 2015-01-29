#pragma once

#include <iostream>

#include <GLTools/glew.h>
#include <GLTools/glfw3.h>

// Handler class for window
class MainWindow
{
public:

	// Run the window. Consumes the thread. 
	static GLint MainWindow::run(GLuint width, GLuint height, const GLchar* title, GLFWmonitor* monitor, GLFWwindow* share);

	// Function binding functions
	static GLvoid bindDraw(GLvoid(*drawIn)(GLfloat));
	static GLvoid bindKeyboard(GLvoid(*keyboradIn)(GLFWwindow*, GLfloat));
	static GLvoid bindInit(GLint(*initIn)());
	static GLvoid bindExit(GLint(*exitIn)());

	static GLvoid bindResize(GLvoid(*resizeIn)(GLFWwindow*, int, int));

	static GLvoid bindScroll(GLvoid(*scrollIn)(GLFWwindow*, GLdouble, GLdouble));

	static GLFWwindow* window;

private:
	
	// function pointers
	static GLvoid(*draw)(GLfloat);
	static GLvoid(*keyboard)(GLFWwindow* window, GLfloat);
	static GLint(*init)();
	static GLint(*exit)();
	static GLvoid(*scroll)(GLFWwindow*, GLdouble, GLdouble);
};