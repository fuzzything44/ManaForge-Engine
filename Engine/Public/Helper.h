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
#include <ios>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

/// <summary> Loads file to string.</summary>
///
/// <param name="filename"> Filename of the file.</param>
///
/// <returns> The file to string.</returns>
ENGINE_API std::string loadFileToStr(const char* filename);

/// <summary> Loads the shaders.</summary>
///
/// <param name="vertexFile">   The vertex file.</param>
/// <param name="fragmentFile"> The fragment file.</param>
///
/// <returns> The shaders.</returns>
ENGINE_API GLuint LoadShaders(const char* vertexFile, const char* fragmentFile);

/// <summary> Loads the DDS.</summary>
///
/// <param name="imagepath"> The imagepath.</param>
///
/// <returns> The DDS.</returns>
ENGINE_API GLuint loadDDS(const char * imagepath);

/// <summary> Gets the string assiociated with the error code.</summary>
///
/// <param name="error"> The error.</param>
///
/// <returns> A the string that is the error.</returns>
ENGINE_API std::string clGetErrorString(cl_int error);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const vec2& vec);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const vec3& vec);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const vec4& vec);


/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const ivec2& vec);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const ivec3& vec);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const ivec4& vec);


/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const uvec2& vec);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const uvec3& vec);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const uvec4& vec);

/// <summary> custom matrix printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const mat3& mat);

/// <summary> custom matrix printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const mat4& mat);

// ONLY define these macros if we are debugging -- they could be slow
#ifdef _DEBUG

/// <summary> A macro that defines error check cl.</summary>
///
/// <param name="err"> The error.</param>
#define errChkCL(err) if(err != CL_SUCCESS){std::stringstream ss; ss << "Error: " << clGetErrorString(err) << "(" << err << ") in file " << __FILE__ << "(" << __LINE__ << ")" << std::endl; throw std::runtime_error(ss.str()); }


/// <summary> Checks a pointer, and throws an exception if it is null.</summary>
#define check(ptr) if(!ptr){ std::stringstream ss; ss << "Null Pointer: " << #ptr << " ("<< __FILE__ << " line: " << __LINE__ << ")\n"; throw std::runtime_error(ss.str() ); };


#else
#define errChkCL(err)
#define check(ptr)
#endif
