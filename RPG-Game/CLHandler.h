#include "Helper.h"

#include <CL/cl.hpp>
#include <CL/cl_gl.h>

#include <vector>

class CLHandler
{
public:

	static cl_int initCL();
	static cl_int updateCL(cl::BufferGL output);
	static cl::Context context;
	static cl::Platform platform;
	static std::vector<cl::Device> devices;

protected:

	static cl::Platform CLHandler::getBestPlatform();
};