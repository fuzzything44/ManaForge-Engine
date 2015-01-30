#pragma once

// OpenGL include
#include <GLTools/glew.h>

// OpenCL include
#include <CL/cl.hpp>

// lodepng include
#include <lodepng/lodepng.h>

// standard library includes
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


// decodes an image and saves it in the vector supplied. With and height are refrences so they can be returned
void decode(std::vector<GLubyte>& image, const GLchar* filename, GLuint& width, GLuint& height);

// saves a png file from the vector supplied to the space filename. Overrides if it already exists
void encodeAndSave(const std::vector<GLubyte>& inPixels, const GLchar* filename, GLuint width, GLuint height);

// saves a png file from the array supplied.
void encodeAndSave(GLubyte* inPixels, const GLchar* filename, GLuint width, GLuint height);

// loads the selected file to a string.
std::string loadFileToStr(const GLchar* filename);

// loads a program from the filepaths specifed
GLuint LoadShaders(const GLchar* vertexFile, const GLchar* fragmentFile);

// loads a texture and returns the ID. width and height are 'returned'
GLuint loadTexture(const GLchar* filepath, GLuint& width, GLuint& height);

// loads a CL program from the filepath using the context and devices
cl::Program loadCLProgram(const GLchar* filepath, cl::Context& context, std::vector<cl::Device>& devices, cl_int* err = NULL);

// OpenCL err checking
inline void errChkCL(cl_int error, const char* name)
{
	if (error != CL_SUCCESS)
	{
		std::cout << "Error: " << name << "(" << error << ")" << std::endl;
		system("pause");
		exit(error);
	}
}
