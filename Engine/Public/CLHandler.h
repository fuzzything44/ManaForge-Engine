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
	static ENGINE_API cl_int initCL();

	/// <param name = 'output'> the buffer to write to </param>
	/// <param name = 'characterLocation'> the current location of the character </param>
	/// <summary> called each frame. </summary>
	static ENGINE_API cl_int updateCL(cl::BufferGL output, cl_float2 characterLocation);

	static ENGINE_API cl::Context* context;
	static ENGINE_API cl::Platform platform;
	static ENGINE_API cl::CommandQueue queue;
	static ENGINE_API std::vector<cl::Device> devices;
	
	/// <summary> Called upon exit -- cleans up openCL resoruces. </summary>
	static ENGINE_API void exitCL();

protected:

	static cl::Platform getBestPlatform();
};