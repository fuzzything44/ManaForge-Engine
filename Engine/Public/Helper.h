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


#ifdef _DEBUG

/// <summary> OpenCL err checking. Returns true is successfull (CL_SUCCESS) </summary>
/// <param name='error'> The error code. </param>
/// <param name='name'> The name of the operation. Used for debugging later. </param>
inline ENGINE_API cl_bool clchk(cl_int error)
{
	if (error != CL_SUCCESS)
	{
		std::string errstring;
		switch (error)
		{
		case CL_DEVICE_NOT_FOUND:
			errstring = "CL_DEVICE_NOT_FOUND";
			break;
		case CL_DEVICE_NOT_AVAILABLE:
			errstring = "CL_DEVICE_NOT_AVAILABLE";
			break;
		case CL_COMPILER_NOT_AVAILABLE:
			errstring = "CL_COMPILER_NOT_AVAILABLE";
			break;
		case CL_MEM_OBJECT_ALLOCATION_FAILURE:
			errstring = "CL_MEM_OBJECT_ALLOCATION_FAILURE";
			break;
		case CL_OUT_OF_RESOURCES:
			errstring = "CL_OUT_OF_RESOURCES";
			break;
		case CL_OUT_OF_HOST_MEMORY:
			errstring = "CL_OUT_OF_HOST_MEMORY";
			break;
		case CL_PROFILING_INFO_NOT_AVAILABLE:
			errstring = "CL_PROFILING_INFO_NOT_AVAILABLE";
			break;
		case CL_MEM_COPY_OVERLAP:
			errstring = "CL_MEM_COPY_OVERLAP";
			break;
		case CL_IMAGE_FORMAT_MISMATCH:
			errstring = "CL_IMAGE_FORMAT_MISMATCH";
			break;
		case CL_IMAGE_FORMAT_NOT_SUPPORTED:
			errstring = "CL_IMAGE_FORMAT_NOT_SUPPORTED";
			break;
		case CL_BUILD_PROGRAM_FAILURE:
			errstring = "CL_BUILD_PROGRAM_FAILURE";
			break;
		case CL_MAP_FAILURE:
			errstring = "CL_MAP_FAILURE";
			break;
		case CL_MISALIGNED_SUB_BUFFER_OFFSET:
			errstring = "CL_MISALIGNED_SUB_BUFFER_OFFSET";
			break;
		case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
			errstring = "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
			break;
		case CL_INVALID_VALUE:
			errstring = "CL_INVALID_VALUE";
			break;
		case CL_INVALID_DEVICE_TYPE:
			errstring = "CL_INVALID_DEVICE_TYPE";
			break;
		case CL_INVALID_PLATFORM:
			errstring = "CL_INVALID_PLATFORM";
			break;
		case CL_INVALID_DEVICE:
			errstring = "CL_INVALID_DEVICE";
			break;
		case CL_INVALID_CONTEXT:
			errstring = "CL_INVALID_CONTEXT";
			break;
		case CL_INVALID_QUEUE_PROPERTIES:
			errstring = "CL_INVALID_QUEUE_PROPERTIES";
			break;
		case CL_INVALID_COMMAND_QUEUE:
			errstring = "CL_INVALID_COMMAND_QUEUE";
			break;
		case CL_INVALID_HOST_PTR:
			errstring = "CL_INVALID_HOST_PTR";
			break;
		case CL_INVALID_MEM_OBJECT:
			errstring = "CL_INVALID_MEM_OBJECT";
			break;
		case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
			errstring = "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
			break;
		case CL_INVALID_IMAGE_SIZE:
			errstring = "CL_INVALID_IMAGE_SIZE";
			break;
		case CL_INVALID_SAMPLER:
			errstring = "CL_INVALID_SAMPLER";
			break;
		case CL_INVALID_BINARY:
			errstring = "CL_INVALID_BINARY";
			break;
		case CL_INVALID_BUILD_OPTIONS:
			errstring = "CL_INVALID_BUILD_OPTIONS";
			break;
		case CL_INVALID_PROGRAM:
			errstring = "CL_INVALID_PROGRAM";
			break;
		case CL_INVALID_PROGRAM_EXECUTABLE:
			errstring = "CL_INVALID_PROGRAM_EXECUTABLE";
			break;
		case CL_INVALID_KERNEL_NAME:
			errstring = "CL_INVALID_KERNEL_NAME";
			break;
		case CL_INVALID_KERNEL_DEFINITION:
			errstring = "CL_INVALID_KERNEL_DEFINITION";
			break;
		case CL_INVALID_KERNEL:
			errstring = "CL_INVALID_KERNEL";
			break;
		case CL_INVALID_ARG_INDEX:
			errstring = "CL_INVALID_ARG_INDEX";
			break;
		case CL_INVALID_ARG_VALUE:
			errstring = "CL_INVALID_ARG_VALUE";
			break;
		case CL_INVALID_ARG_SIZE:
			errstring = "CL_INVALID_ARG_SIZE";
			break;
		case CL_INVALID_KERNEL_ARGS:
			errstring = "CL_INVALID_KERNEL_ARGS";
			break;
		case CL_INVALID_WORK_DIMENSION:
			errstring = "CL_INVALID_WORK_DIMENSION";
			break;
		case CL_INVALID_WORK_GROUP_SIZE:
			errstring = "CL_INVALID_WORK_GROUP_SIZE";
			break;
		case CL_INVALID_WORK_ITEM_SIZE:
			errstring = "CL_INVALID_WORK_ITEM_SIZE";
			break;
		case CL_INVALID_GLOBAL_OFFSET:
			errstring = "CL_INVALID_GLOBAL_OFFSET";
			break;
		case CL_INVALID_EVENT_WAIT_LIST:
			errstring = "CL_INVALID_EVENT_WAIT_LIST";
			break;
		case CL_INVALID_EVENT:
			errstring = "CL_INVALID_EVENT";
			break;
		case CL_INVALID_OPERATION:
			errstring = "CL_INVALID_OPERATION";
			break;
		case CL_INVALID_GL_OBJECT:
			errstring = "CL_INVALID_GL_OBJECT";
			break;
		case CL_INVALID_BUFFER_SIZE:
			errstring = "CL_INVALID_BUFFER_SIZE";
			break;
		case CL_INVALID_MIP_LEVEL:
			errstring = "CL_INVALID_MIP_LEVEL";
			break;
		case CL_INVALID_GLOBAL_WORK_SIZE:
			errstring = "CL_INVALID_GLOBAL_WORK_SIZE";
			break;
		case CL_INVALID_PROPERTY:
			errstring = "CL_INVALID_PROPERTY";
			break;
		case CL_PLATFORM_NOT_FOUND_KHR:
			errstring = "CL_PLATFORM_NOT_FOUND";
			break;
		default:
			errstring = "Unspecified error";
		}

		ENG_LOG("Error: " << errstring << "(" << error << ") In file " << __FILE__ << " line " << __LINE__ << std::endl);

		return false;
	}
	else
	{
		return true;
	}

}

#define errChkCL(err) if(!clchk(err)) exit(err)
	
#else
#define errChkCL(err)
#endif