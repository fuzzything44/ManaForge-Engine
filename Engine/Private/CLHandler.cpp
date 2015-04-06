#include "stdafx.h"
#include "CLHandler.h"

// init static variables
cl::Context* CLHandler::context = nullptr;
cl::Platform CLHandler::platform = nullptr;
cl::CommandQueue* CLHandler::queue = nullptr;
cl::Program* CLHandler::updateProgram = nullptr;
cl::Kernel CLHandler::collideKern = cl::Kernel();
cl::Kernel CLHandler::updateKern = cl::Kernel();
cl::Buffer CLHandler::actors = cl::Buffer();
cl::BufferGL CLHandler::posCLBuffer = cl::BufferGL();
cl::BufferGL CLHandler::UVCLBuffer = cl::BufferGL();
cl::BufferGL CLHandler::elemCLBuffer = cl::BufferGL();
std::vector<cl::Device> CLHandler::devices = std::vector<cl::Device>();

cl_int CLHandler::updateCL(float deltaTime, std::vector<ActorData>& data)
{STACK

	check(context);
	check(updateProgram);

	auto objects = new std::vector<cl::Memory>();
	objects->push_back(posCLBuffer);
	objects->push_back(UVCLBuffer);
	objects->push_back(elemCLBuffer);

	glFinish();

	cl_int err = CL_SUCCESS;

	// acquire GL objects so we can write to them
	err = queue->enqueueAcquireGLObjects(objects);
	//queue->finish();
	errChkCL(err);

	// copy the actor data -- will later only copy needed.
	err = queue->enqueueWriteBuffer(actors, true, 0, sizeof(ActorData) * data.size(), &data[0]);
	errChkCL(err);
	err = queue->finish();
	errChkCL(err);


	updateKern.setArg(0, posCLBuffer);
	updateKern.setArg(1, UVCLBuffer);
	updateKern.setArg(2, elemCLBuffer);
	updateKern.setArg(3, actors);
	updateKern.setArg(4, deltaTime);

	queue->finish();
	errChkCL(err);
	err = queue->enqueueNDRangeKernel(updateKern, cl::NullRange, cl::NDRange(data.size()), cl::NullRange);
	errChkCL(err);
	err = queue->finish();
	errChkCL(err);

	// copy the actor data -- will later only copy needed.
	err = queue->enqueueReadBuffer(actors, true, 0, sizeof(ActorData) * data.size(), &data[0]);
	errChkCL(err);
	err = queue->finish();
	errChkCL(err);

	// copy data back
	for (auto& elem : data)
	{
		elem.actorPtr->data = elem;
	}

	// release objects back to opnegl
	queue->enqueueReleaseGLObjects(objects);

	return CL_SUCCESS;
}

void CLHandler::wait()
{STACK
	queue->flush();
}

cl_int CLHandler::initCL(GLuint posBuffer, GLuint UVBuffer, GLuint elemBuffer)
{STACK


	platform = getBestPlatform();

	ENG_LOG("Using Platform: " << platform.getInfo<CL_PLATFORM_NAME>() << std::endl);

	platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
	

	ENG_LOG("Using Device: " << devices[0].getInfo<CL_DEVICE_NAME>() << std::endl);


	// the way you create a Context is platform dependent sooooo
#ifdef _WIN32

	// define properies for the context
	cl_context_properties context_properties[] =
	{
		CL_GL_CONTEXT_KHR, reinterpret_cast<cl_context_properties>(wglGetCurrentContext()), // use the openGL context so we can share buffers
		CL_WGL_HDC_KHR, reinterpret_cast<cl_context_properties>(wglGetCurrentDC()),			// use the openGL DC
		CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(platform()),			// use the platform that we decided was fastest
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
	context = new cl::Context(devices, context_properties, nullptr, nullptr, &err);

	// make sure the context was created successfully
	errChkCL(err);

	ENG_LOG(std::endl << "Loading Program updateProgram");

	// load the program
	err = loadCLProgram(UPDATE_LOCATION, updateProgram);

	ENG_LOG(std::endl << "Program Loaded" << std::endl)

	ENG_LOG(std::endl << "Loading kernel: collide from program: ");

	// load the kernel from the program
	collideKern = cl::Kernel(*updateProgram, "collide", &err);

	errChkCL(err);

	ENG_LOG("Completed" << std::endl);

	ENG_LOG(std::endl << "Loading kernel: update from program" << std::endl);


	updateKern = cl::Kernel(*updateProgram, "update", &err);


	ENG_LOG("Kernel Loading completed" << std::endl);

	// do error checking
	errChkCL(err);

	ENG_LOG(std::endl << "Creating Command Queue: ");
	
	// init the queue
	queue = new cl::CommandQueue(*context, NULL, &err);

	ENG_LOG("completed" << std::endl);

	// do error checking
	errChkCL(err);

	// init the buffer
	actors = cl::Buffer(*context, static_cast<cl_mem_flags>(CL_MEM_READ_WRITE), sizeof(ActorData) * MAX_ACTORS, nullptr, &err);
		
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

void CLHandler::exitCL()
{STACK
	clReleaseCommandQueue((*queue)());
	clReleaseContext((*context)());
}

cl::Platform CLHandler::getBestPlatform()
{STACK

	std::vector<cl::Platform> platforms;
	std::vector<cl::Device> devices;

	cl::Platform ret;

	cl::Platform::get(&platforms);
	
	cl_int fastestPlatform = 0;

	for (auto& p : platforms)
	{
		p.getDevices(CL_DEVICE_TYPE_ALL, &devices);

		for (auto& d : devices)
		{
			cl_int speed;
			d.getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &speed);

			if (speed > fastestPlatform)
			{
				fastestPlatform = speed;
				ret = p;
			}
		}
	}
	return ret;
}

cl_int CLHandler::loadCLProgram(const GLchar* filepath, cl::Program*& program)
{STACK
	

	ENG_LOG(std::endl << std::endl);

	int err = CL_SUCCESS;

	// load kernel
	ENG_LOG("Compiling Program " << filepath << std::endl);

	auto source = loadFileToStr(filepath);

	program = new cl::Program();
	*program = cl::Program(*context, source, false, &err);

	errChkCL(err)

	err = program->build(devices);

	if (err != CL_SUCCESS)
	{
		//TODO: Fix
		ENG_LOG(" Error building: " << program->getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]) << "\t" << err << "\n");
		return err;
	}
	else
	{
		ENG_LOG("Program successfully compiled" << std::endl);
	}

	return err;
}