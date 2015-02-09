#pragma once

// OpenGL include
#include <GLTools/glew.h>

// OpenCL include
#include <CL/cl.hpp>


// standard library includes
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII



/// <summary> Loads the selected file to a string.
/// <para> Returns a string that contains the data </para>
/// </summary>
/// <param name='filename'> The path to the file. </param>
std::string loadFileToStr(const GLchar* filename);

/// <summary> loads a program from the filepaths specifed </summary>
/// <param name='vertexFile'> The path to the vertex shader. Must contain an extension. </param>
/// <param name='fragmentFile'> The path to the fragment shader. Must contain an extension. </param>
GLuint LoadShaders(const GLchar* vertexFile, const GLchar* fragmentFile);

/// <summary> loads a CL program and returns it </summary>
/// <param name='filepath'> The path to the file that contains the kernels. Must have an extension. </param>
/// <param name='context'> The context to create the program in </param>
/// <param name='devices'> A vector of cl::Device that has the devices to comile it on. </param>
/// <param name='err'> A cl_int* that is set to the error code. Suggested for error handling. </param>
cl::Program loadCLProgram(const GLchar* filepath, cl::Context& context, std::vector<cl::Device>& devices, cl_int* err = NULL);

/// <summary> load a DDS format texture to the GLuint that is returned </summary>
/// <param name='imagepath'> the pathe to the DDS file </param>
GLuint loadDDS(const GLchar * imagepath);

/// <summary> load a DSS format to the texture specified at the location specified </summary>
/// <param name='texToAppend'> The handle for the texture to append the image to </param>
/// <param name='Xoffset'> The X offset of the original texture to place the image </param>
/// <param name='Yoffset'> The Y offset of the original texture to place the image </param>
/// <param name='filepath'> The path to the DDS file. </param>
void appendDDS(GLuint texToAppend, GLuint Xoffset, GLuint Yoffset, const GLchar* filepath);

/// <summary> allocates an empty texture library from the format of the given DDS file </summary>
/// <param name='num'> The amount of textures in one direction. total textures is up to num squared </param>
/// <param name='filepath'> The path to the DDS file to load </param>
GLuint allocateCompressedTextureLibraryFromDDS(GLuint num, const char* filepath);

/// <summary> OpenCL err checking. Returns true is successfull (CL_SUCCESS) </summary>
/// <param name='error'> The error code. </param>
/// <param name='name'> The name of the operation. Used for debugging later. </param>
inline cl_bool errChkCL(cl_int error, const char* name)
{
	if (error != CL_SUCCESS)
	{
		std::cout << "Error: " << name << "(" << error << ")" << std::endl;
		return false;
	}
	else
	{
		return true;
	}

}
