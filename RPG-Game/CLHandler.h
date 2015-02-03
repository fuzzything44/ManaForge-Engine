#pragma once

#include "Helper.h"

#include <CL/cl.hpp>
#include <CL/cl_gl.h>

#include <vector>

// Class to do all the OpenCL calculations. 
class CLHandler
{
public:

	/// <summary> initialize the context, platform and devices. Returns an error code, CL_SUCCESS for success </summary>
	static cl_int initCL();

	/// <param name = 'output'> the buffer to write to </param>
	/// <param name = 'characterLocation'> the current location of the character </param>
	/// <summary> called each frame. </summary>
	static cl_int updateCL(cl::BufferGL output, cl_float2 characterLocation);

	static cl::Context context;
	static cl::Platform platform;
	static std::vector<cl::Device> devices;

protected:

	static cl::Platform CLHandler::getBestPlatform();
};