#pragma once

#include <GLTools/glew.h>

#include <CL/cl.hpp>

#include <lodepng/lodepng.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>



void decode(std::vector<GLubyte>& image, const GLchar* filename, GLuint& width, GLuint& height);
void encodeAndSave(const std::vector<GLubyte>& inPixels, const GLchar* filename, GLuint width, GLuint height);
void encodeAndSave(GLubyte* inPixels, const GLchar* filename, GLuint width, GLuint height);
std::string loadFileToStr(const GLchar* filename);
GLuint LoadShaders(const GLchar* vertexFile, const GLchar* fragmentFile);
GLuint loadTexture(const GLchar* filepath, GLuint& width, GLuint& height);
GLvoid setActiveTexture(GLuint ID);
cl::Program loadProgram(const GLchar* filepath, cl::Context& context, std::vector<cl::Device>& devices, cl_int* err = NULL);

// opencl err checking
inline void errChkCL(cl_int error, const char* name)
{
	if (error != CL_SUCCESS)
	{
		std::cout << "Error: " << name << "(" << error << ")" << std::endl;
		system("pause");
		exit(error);
	}
}
