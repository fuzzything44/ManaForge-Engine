#pragma once

#include <GLTools/glew.h>

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