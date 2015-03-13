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

/**
 * \fn	ENGINE_API std::string loadFileToStr(const char* filename);
 *
 * \brief	Loads file to string.
 *
 * \author	Russell
 * \date	3/12/2015
 *
 * \param	filename	Filename of the file.
 *
 * \return	The file to string.
 */

ENGINE_API std::string loadFileToStr(const char* filename);

/**
 * \fn	ENGINE_API GLuint LoadShaders(const char* vertexFile, const char* fragmentFile);
 *
 * \brief	Loads the shaders.
 *
 * \author	Russell
 * \date	3/12/2015
 *
 * \param	vertexFile  	The vertex file.
 * \param	fragmentFile	The fragment file.
 *
 * \return	The shaders.
 */

ENGINE_API GLuint LoadShaders(const char* vertexFile, const char* fragmentFile);

/**
 * \fn	ENGINE_API GLuint loadDDS(const char * imagepath);
 *
 * \brief	Loads the DDS.
 *
 * \author	Russell
 * \date	3/12/2015
 *
 * \param	imagepath	The imagepath.
 *
 * \return	The DDS.
 */

ENGINE_API GLuint loadDDS(const char * imagepath);

/**
 * \fn	ENGINE_API std::string clGetErrorString(cl_int error);
 *
 * \brief	Cl get error string.
 *
 * \author	Russell
 * \date	3/12/2015
 *
 * \param	error	The error.
 *
 * \return	A std::string.
 */

ENGINE_API std::string clGetErrorString(cl_int error);

// custom vector printing
ENGINE_API std::ostream& operator<<(std::ostream& os, const vec2& vec);

// custom vector printing
ENGINE_API std::ostream& operator<<(std::ostream& os, const vec3& vec);

// custom vector printing
ENGINE_API std::ostream& operator<<(std::ostream& os, const vec4& vec);


// custom vector printing
ENGINE_API std::ostream& operator<<(std::ostream& os, const ivec2& vec);

// custom vector printing
ENGINE_API std::ostream& operator<<(std::ostream& os, const ivec3& vec);

// custom vector printing
ENGINE_API std::ostream& operator<<(std::ostream& os, const ivec4& vec);


// custom vector printing
ENGINE_API std::ostream& operator<<(std::ostream& os, const uvec2& vec);

// custom vector printing
ENGINE_API std::ostream& operator<<(std::ostream& os, const uvec3& vec);

// custom vector printing
ENGINE_API std::ostream& operator<<(std::ostream& os, const uvec4& vec);



// custom matrix printing
ENGINE_API std::ostream& operator<<(std::ostream& os, const mat3& mat);

// custom matrix printing
ENGINE_API std::ostream& operator<<(std::ostream& os, const mat4& mat);


#ifdef _DEBUG

/**
 * \def	errChkCL(err) if(err != CL_SUCCESS)
 *
 * \brief	A macro that does CL error checking
 *
 * \author	Russell
 * \date	3/12/2015
 *
 * \param	err	The error.
 */

#define errChkCL(err) if(err != CL_SUCCESS){ENG_LOG("Error: " << clGetErrorString(err) << "(" << err << ") in file " << __FILE__ << "(" << __LINE__  << ")" << std::endl);exit(err);} else {err = CL_SUCCESS;}
	
#else
#define errChkCL(err)
#endif


// quickly checks if the ptr is valid, and returns if not.
#define check(ptr) if(!ptr) return -1;