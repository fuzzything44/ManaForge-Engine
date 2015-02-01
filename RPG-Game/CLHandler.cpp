#include "CLHandler.h"

cl::Context CLHandler::context = cl::Context();
cl::Platform CLHandler::platform = cl::Platform();
std::vector<cl::Device> CLHandler::devices = std::vector<cl::Device>();

cl_int CLHandler::updateCL(cl::BufferGL output, cl_float2 characterLocation)
{
	

	return CL_SUCCESS;
}

cl_int CLHandler::initCL()
{
	platform = getBestPlatform();

	std::cout << "Using Platform: " << platform.getInfo<CL_PLATFORM_NAME>() << std::endl;

	platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
	if (!errChkCL(devices.size() != 0 ? CL_SUCCESS : CL_DEVICE_NOT_AVAILABLE, "platform.getDevices"))
	{
		return CL_DEVICE_NOT_AVAILABLE;
	}


	std::cout << "Using Device: " << devices[0].getInfo<CL_DEVICE_NAME>() << std::endl;


	cl_context_properties context_properties[] =
	{
		CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform(),
		NULL
	};

	cl_int err = CL_SUCCESS;
	context = cl::Context(devices, context_properties, NULL, NULL, &err);

	if (!errChkCL(err, "Create Context")){
		return err;
	}
	

	return CL_SUCCESS;
}

cl::Platform CLHandler::getBestPlatform()
{

	std::vector<cl::Platform> platforms;
	std::vector<cl::Device> devices;

	cl::Platform ret;

	cl::Platform::get(&platforms);

	// make sure there is a platform
	errChkCL(platforms.size() == 0 ? -1 : CL_SUCCESS, "cl::Platform::get");


	cl_int fastestNum = 0;

	for (auto& p : platforms)
	{
		p.getDevices(CL_DEVICE_TYPE_ALL, &devices);

		for (auto& d : devices)
		{
			cl_int speed;
			d.getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &speed);

			if (speed > fastestNum)
			{
				fastestNum = speed;
				ret = p;
			}
		}
	}
	return ret;
}