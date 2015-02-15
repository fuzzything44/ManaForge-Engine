#pragma once
#include "Engine.h"

#include "Helper.h"

#include <CL/cl.hpp>
#include <CL/cl_gl.h>

#include <vector>

// Class to do all the OpenCL calculations. 
class CLHandler
{
public:

	/// <summary> initialize the context, platform and devices. Returns an error code, CL_SUCCESS for success </summary>
	static ENGINE_API cl_int initCL(cl_context_properties *contextProps);

	/// <param name = 'output'> the buffer to write to </param>
	/// <param name = 'characterLocation'> the current location of the character </param>
	/// <summary> called each frame. </summary>
	static ENGINE_API cl_int updateCL(cl::BufferGL output, cl_float2 characterLocation);

	static cl::Context context;
	static cl::Platform platform;
	static cl::CommandQueue queue;
	static std::vector<cl::Device> devices;
	static ENGINE_API void exitCL();

protected:

	static cl::Platform getBestPlatform();
};