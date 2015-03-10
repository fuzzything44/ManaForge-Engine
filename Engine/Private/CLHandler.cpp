#include "stdafx.h"
#include "CLHandler.h"

// init static variables
cl::Context* CLHandler::context = NULL;
cl::Platform CLHandler::platform = NULL;
cl::CommandQueue* CLHandler::queue = NULL;
cl::Program* CLHandler::updateProgram = NULL;
cl::Kernel CLHandler::collideKern = cl::Kernel();
cl::Kernel CLHandler::updateKern = cl::Kernel();
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
	std::vector<cl::Memory>* objects = new std::vector<cl::Memory>();
	objects->push_back(posCLBuffer);
	objects->push_back(UVCLBuffer);
	objects->push_back(elemCLBuffer);

	cl_int err = CL_SUCCESS;
	
	// acquire GL objects so we can write to them
	err = queue->enqueueAcquireGLObjects(objects);
	queue->finish();
	errChkCL(err);

	// copy the actor data -- will later only copy needed.
	err = queue->enqueueWriteBuffer(actors, true, 0, sizeof(ActorData) * data.size(), &data[0]);
	queue->finish();
	errChkCL(err);


	err = queue->enqueueNDRangeKernel(updateKern, cl::NullRange, cl::NDRange(data.size()), cl::NDRange(1), NULL, );
	queue->finish();
	errChkCL(err);

	// release objects back to opnegl
	queue->enqueueReleaseGLObjects(objects);

	return CL_SUCCESS;
}

cl_int CLHandler::initCL(GLuint posBuffer, GLuint UVBuffer, GLuint elemBuffer)
{

	platform = getBestPlatform();

	ENG_LOG("Using Platform: " << platform.getInfo<CL_PLATFORM_NAME>() << std::endl);

	platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
	

	ENG_LOG("Using Device: " << devices[0].getInfo<CL_DEVICE_NAME>() << std::endl);


	// the way you create a Context is platform dependent sooooo
#ifdef _WIN32

	// define properies for the context
	cl_context_properties context_properties[] =
	{
		CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(), // use the openGL context so we can share buffers
		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(), // use the openGL DC
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform(), // use the platform that we decided was fastest
		NULL // add a null at end so we don't get null ptr exception.
	};

#endif
#ifdef __linux__
	cl_context_properties context_properties[] =
	{
		{
			CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
			CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
			CL_CONTEXT_PLATFORM, (cl_context_properties)Platform,
			NULL
		};
		
#endif 
#ifdef __APPLE__
		cl_context_properties context_properties[] =
		{
			CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
			(cl_context_properties)kCGLShareGroup,
			0
		};
#endif

	// create the context --  store error in err
	cl_int err = CL_SUCCESS;
	context = new cl::Context(devices, context_properties, NULL, NULL, &err);

	// make sure the context was created successfully
	errChkCL(err);

	// load the program
	err = loadCLProgram(UPDATE_LOCATION, updateProgram);

	ENG_LOG(std::endl << "Loading kernel: collide from program" << std::endl);

	// load the kernel from the program
	collideKern = cl::Kernel(*updateProgram, "collide", &err);

	errChkCL(err);

	updateKern = cl::Kernel(*updateProgram, "update", &err);

	// do error checking
	errChkCL(err);


	ENG_LOG("Kernel Loading completed" << std::endl);
	

	// init the queue
	queue = new cl::CommandQueue(*context, NULL, &err);

	// do error checking
	errChkCL(err);

	// init the buffer
	actors = cl::Buffer(*context, (cl_mem_flags)CL_MEM_READ_WRITE, (size_t)sizeof(ActorData) * 100, (void*)0, &err);
		
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
	//return platforms[1];
	
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

	errChkCL(err)

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
