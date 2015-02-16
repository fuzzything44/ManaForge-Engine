#pragma once
#include "Engine.h"

#include "Helper.h"

#include <CL/cl.hpp>
#include <CL/cl_gl.h>

#include <vector>

static const const char* UPDATE_LOCATION = "kernels/updateFrame.cl";

// Class to do all the OpenCL calculations. 
class CLHandler
{
public:

	/// <summary> initialize the context, platform and devices. Returns an error code, CL_SUCCESS for success </summary>
	static ENGINE_API cl_int initCL();

	/// <param name = 'locBuffer'> the buffer to write to </param>
	/// <param name='UVBuffer'> The Buffer to write UV data to </param>
	/// <param name='elemBuffer'> The element buffer object to write to </param>
	/// <param name = 'characterLocation'> the current location of the character </param>
	/// <summary> called each frame. </summary>
	static ENGINE_API cl_int updateCL(cl::BufferGL locBuffer, cl::BufferGL UVBuffer, cl::BufferGL elemBuffer, cl_float2 characterLocation);

	/// <summary> Called upon exit -- cleans up openCL resoruces. </summary>
	static ENGINE_API void exitCL();

protected:

	// pointer to the context -- needs to be a pointer or else it will freeze on exit . Will be null before initCL();
	static cl::Context* context;

	// the platform to use. Will be null before initCL()
	static cl::Platform platform;

	// the queue to use. Wil be invalid before initCL()
	static cl::CommandQueue queue;

	// a list of the devices of platform
	static std::vector<cl::Device> devices;

	// the program the contains the entire code for updating the frame
	static cl::Program* updateProgram;

	// the kernel used for collision
	static cl::Kernel collideKern;


	/// <summary> loads a CL program and returns the error code </summary>
	/// <param name='filepath'> The path to the file that contains the kernels. Must have an extension. </param>
	/// <param name='program'> Pointer to the program to write to </param>
	static cl_int CLHandler::loadCLProgram(const GLchar* filepath, cl::Program*& program);

	static cl::Platform getBestPlatform();
};