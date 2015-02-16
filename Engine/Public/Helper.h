#pragma once
#include "Engine.h"

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
ENGINE_API std::string loadFileToStr(const GLchar* filename);

/// <summary> loads a program from the filepaths specifed </summary>
/// <param name='vertexFile'> The path to the vertex shader. Must contain an extension. </param>
/// <param name='fragmentFile'> The path to the fragment shader. Must contain an extension. </param>
ENGINE_API GLuint LoadShaders(const GLchar* vertexFile, const GLchar* fragmentFile);



/// <summary> load a DDS format texture to the GLuint that is returned </summary>
/// <param name='imagepath'> the pathe to the DDS file </param>
ENGINE_API GLuint loadDDS(const GLchar * imagepath);

/// <summary> OpenCL err checking. Returns true is successfull (CL_SUCCESS) </summary>
/// <param name='error'> The error code. </param>
/// <param name='name'> The name of the operation. Used for debugging later. </param>
inline ENGINE_API cl_bool errChkCL(cl_int error, const char* name)
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
