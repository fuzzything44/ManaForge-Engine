#include "stdafx.h"
#include "CLHandler.h"

// init static variables
cl::Context* CLHandler::context = NULL;
cl::Platform CLHandler::platform = NULL;
cl::CommandQueue* CLHandler::queue = NULL;
cl::Program* CLHandler::updateProgram = NULL;
cl::Kernel CLHandler::collideKern = cl::Kernel();
cl::Buffer CLHandler::actors = cl::Buffer();
cl::BufferGL CLHandler::posCLBuffer = cl::BufferGL();
cl::BufferGL CLHandler::UVCLBuffer = cl::BufferGL();
cl::BufferGL CLHandler::elemCLBuffer = cl::BufferGL();
std::vector<cl::Device> CLHandler::devices = std::vector<cl::Device>();


/// <param name = 'locBuffer'> the buffer to write to </param>
/// <param name = 'characterLocation'> the current location of the character </param>
/// <summary> called each frame. </summary>
cl_int CLHandler::updateCL(vec2 characterLocation, std::vector<ActorData>& data)
{
	std::vector<cl::Memory> objects = { posCLBuffer, UVCLBuffer, elemCLBuffer };

	cl_int err = CL_SUCCESS;
	
	// acquire GL objects
	err = queue->enqueueAcquireGLObjects(&objects);

	errChkCL(err);

	err = queue->enqueueWriteBuffer(actors, true, 0, sizeof(ActorData) * data.size(), &data[0]);

	errChkCL(err);

	return CL_SUCCESS;
}

cl_int CLHandler::initCL(GLuint posBuffer, GLuint UVBuffer, GLuint elemBuffer)
{

	platform = getBestPlatform();

	ENG_LOG("Using Platform: " << platform.getInfo<CL_PLATFORM_NAME>() << std::endl);

	platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

	errChkCL(devices.size() != 0 ? CL_SUCCESS : CL_DEVICE_NOT_AVAILABLE);


	ENG_LOG("Using Device: " << devices[0].getInfo<CL_DEVICE_NAME>() << std::endl);


	// define properies for the context
	cl_context_properties context_properties[] =
	{
		CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(), // use the openGL context so we can share buffers
		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(), // use the openGL DC
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform(), // use the platform that we decided was fastest
		NULL // add a null at end so we don't get null ptr exception.
	};

	// create the context --  store error in err
	cl_int err = CL_SUCCESS;
	context = new cl::Context(devices, context_properties, NULL, NULL, &err);

	// make sure the context was created successfully
	errChkCL(err);

	// load the program
	err = loadCLProgram(UPDATE_LOCATION, updateProgram);

	ENG_LOG(std::endl << "Loading kernel: collide from program" << std::endl);

	// load the kernel from the program
	cl::Kernel Kern(*updateProgram, "collide", &err);

	// do error checking
	errChkCL(err);


	ENG_LOG("Kernel Loading completed" << std::endl);
	

	// init the queue
	queue = new cl::CommandQueue(*context, NULL, &err);

	// do error checking
	errChkCL(err);

	// init the buffer
	actors = cl::Buffer(*context, (cl_mem_flags)CL_MEM_READ_WRITE, NULL, &err);	
	
	// do error checking
	errChkCL(err);

	posCLBuffer = cl::BufferGL(*context, CL_MEM_WRITE_ONLY, posBuffer, &err);

	// do error checking
	errChkCL(err);


	UVCLBuffer = cl::BufferGL(*context, CL_MEM_WRITE_ONLY, UVBuffer, &err);

	// do error checking
	errChkCL(err);

	elemCLBuffer = cl::BufferGL(*context, CL_MEM_WRITE_ONLY, elemBuffer, &err);

	// do error checking
	errChkCL(err);

	posCLBuffer = cl::BufferGL();


	return CL_SUCCESS;
}

/// <summary> Called upon exit -- cleans up openCL resoruces. </summary>
void CLHandler::exitCL()
{
	clReleaseCommandQueue((*queue)());
	clReleaseContext((*context)());
}

cl::Platform CLHandler::getBestPlatform()
{

	std::vector<cl::Platform> platforms;
	std::vector<cl::Device> devices;

	cl::Platform ret;

	cl::Platform::get(&platforms);

	// make sure there is a platform
	errChkCL(platforms.size() == 0 ? CL_PLATFORM_NOT_FOUND_KHR : CL_SUCCESS);
	
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

/// <summary> loads a CL program and returns the error code </summary>
/// <param name='filepath'> The path to the file that contains the kernels. Must have an extension. </param>
/// <param name='program'> 
/// <para> refrence to the pointer to the program to write to <para>
/// <para> -- this way it saves the memory address back to the pointer given <para>
/// </param>
cl_int CLHandler::loadCLProgram(const GLchar* filepath, cl::Program*& program)
{
	ENG_LOG(std::endl << std::endl);

	int err = CL_SUCCESS;

	// load kernel
	ENG_LOG("Compiling Program " << filepath << std::endl);

	std::string source = loadFileToStr(filepath);

	program = new cl::Program();
	*program = cl::Program(*context, source, false, &err);

	errChkCL(err);

	err = program->build(devices);

	if (err != CL_SUCCESS)
	{
		ENG_LOG(" Error building: " << program->getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]) << "\t" << err << "\n");
		return err;
	}
	else
	{
		ENG_LOG("Program successfully compiled" << std::endl);
	}

	return err;
}