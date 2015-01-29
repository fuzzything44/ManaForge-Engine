// opengl includes
#include <GLTools/glew.h>
#include <GLTools/glfw3.h>

// opengl includes
#include <CL/cl.hpp> // .hpp has some object oriented programming approaches to it
#include <CL/cl_gl.h> // interop code

#include "MainWindow.h"
#include "Helper.h"

/*************** P R O T O T Y P E S ***************************************************/
GLvoid draw(GLfloat delta);
GLint init();

cl::Platform getBestPlatform();
cl_int initCL();


GLint main()
{
	// bind the draw function to the draw function
	MainWindow::bindDraw(draw);

	// bind the init function to init
	MainWindow::bindInit(init);

	// run the window. consumes the thread until it returns
	return MainWindow::run(800, 600, "RPG-Simulator", NULL, NULL);
}

GLvoid draw(GLfloat delta)
{
	// clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT);
}

GLint init()
{
	// set a background color -- it is in 0-1 scale. Pink is the best.
	glClearColor(1.f, .2f, .5f, 1.f);
	



	// init openCL. opencl has to be initalized after opengl and all of our buffers. 
	cl_int err = initCL();

	// return error code. Zero for success
	return 0;
}

cl_int initCL()
{
	cl::Platform platform = getBestPlatform();

	std::cout << "Using Platform: " << platform.getInfo<CL_PLATFORM_NAME>() << std::endl;

	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
	errChkCL(devices.size() != 0 ? CL_SUCCESS : -1, "Get Devices");

	std::cout << "Using Device: " << devices[0].getInfo<CL_DEVICE_NAME>() << std::endl;


	cl_context_properties context_properties[] =
	{
		CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform(),
		NULL
	};

	cl_int err = CL_SUCCESS;
	cl::Context context(devices, context_properties, NULL, NULL, &err);

	if (err != CL_SUCCESS){
		std::cout << "Error creating context" << "\t" << err << "\n";
		system("pause");
		exit(-1);
	}
	//cl::Program program = loadProgram("kernel.cl", context, devices, &err);


	return CL_SUCCESS;
}

cl::Platform getBestPlatform()
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