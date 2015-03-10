#pragma once
#include <Engine.h>
#include <Logging.h>

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
ENGINE_API std::string loadFileToStr(const char* filename);

/// <summary> loads a program from the filepaths specifed </summary>
/// <param name='vertexFile'> The path to the vertex shader. Must contain an extension. </param>
/// <param name='fragmentFile'> The path to the fragment shader. Must contain an extension. </param>
ENGINE_API GLuint LoadShaders(const char* vertexFile, const char* fragmentFile);



/// <summary> load a DDS format texture to the GLuint that is returned </summary>
/// <param name='imagepath'> the pathe to the DDS file </param>
ENGINE_API GLuint loadDDS(const char * imagepath);



ENGINE_API std::string clGetErrorString(cl_int error);



#ifdef _DEBUG

#define errChkCL(err) if(err != CL_SUCCESS){ENG_LOG("Error: " << clGetErrorString(err) << "(" << err << ") in file " << __FILE__ << "(" << __LINE__  << ")" << std::endl);exit(err);} else {err = CL_SUCCESS;}
	
#else
#define errChkCL(err)
#endif