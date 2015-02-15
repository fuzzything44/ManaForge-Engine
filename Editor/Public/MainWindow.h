#pragma once
#include <iostream>

#include <GLTools/glew.h>
#include <GLTools/glfw3.h>

#include <glm/glm.hpp>

// Handler class for window
class MainWindow
{
public:

	/// <summary> Runs the Thread. Consumes the thread and returns the error code. </summary>
	/// <param name='title'> The title of the winodw </param>
	/// <param name='isFullscreen'> If the window should be fullscree or not </param>
	static GLint MainWindow::run(const GLchar* title, GLboolean isFullscreen);

	/// <summary> Bind the function to be called each frame to draw the elements.
	/// <para> Called after the keyboard function </para>
	/// </summary>
	/// <param name='drawIn'> the function pointer. Takes in a GLfloat value that represents the Delta Time </param>
	static GLvoid bindDraw(GLvoid(*drawIn)(GLfloat));

	/// <summary> Bind the function to be called each frame to handle input. 
	/// <para> Called before the draw function </para>
	/// </summary>
	/// <param name='keyboardIn'> The function pointer. Inputs: 
	/// <para> GLFWwinodw* - the current winodw that input should be tested aginst. </para>
	/// <para> GLfloat - delta time </para>
	/// </param>
	static GLvoid bindKeyboard(GLvoid(*keyboardIn)(GLFWwindow*, GLfloat));

	/// <summary> Bind the function to be called on initalization. 
	/// <para> Called after all OpenGL and GLFW initalization. </para>
	/// </summary>
	/// <param name='initIn'> The function pointer. Returns a GLint for error code </param>
	static GLvoid bindInit(GLint(*initIn)());

	/// <summary> Bind the function to be called right before exiting the program.
	/// <para> Used for clenup and memory freeing. </para>
	/// </summary>
	/// <param name='exitIn'> The function pointer. Returns a GLint for error code </param>
	static GLvoid bindExit(GLint(*exitIn)());

	/// <summary> Bind the function that is called when the window is resized. </summary>
	/// <param name='resizeIn'> The function pointer. Inputs:
	/// <para> GLFWwinodw* - the window that was resized. </para>
	/// <para> int - the new width of the window. </para>
	/// <para> int - the new height of the winodw. </para>
	/// </param>
	static GLvoid bindResize(GLvoid(*resizeIn)(GLFWwindow*, int, int));

	/// <summary> Bind the function that is called when the scroll wheel is turned.
	/// <para> Called every frame </para>
	/// </summary>
	/// <param name='scrollIn'> The function pointer. Inputs:
	/// <para> GLFWwinodw* - The window that currently has focus. </para>
	/// <para> GLdouble - The X scroll wheel coordinate. 1 for one click, -1 for one click the oppossite direction, etc. </para>
	/// <para> GLdouble - The Y scroll wheel coordinate. 1 for one click, -1 for one click the oppossite direction, etc. </para>
	/// </param>
	static GLvoid bindScroll(GLvoid(*scrollIn)(GLFWwindow*, GLdouble, GLdouble));

	static GLint getWindowWidth();
	static GLint getWindowHeight();

private:


	static GLFWwindow* window;
	
	// function pointers
	static GLvoid(*draw)(GLfloat);
	static GLvoid(*keyboard)(GLFWwindow* window, GLfloat);
	static GLint(*init)();
	static GLint(*exit)();
	static GLvoid(*scroll)(GLFWwindow*, GLdouble, GLdouble);
};