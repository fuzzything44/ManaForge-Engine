

#pragma once
#include "Engine.h"

#include "Helper.h"
#include "Logging.h"

class CLHandler;
#include "Actor.h"

#include <CL/cl.hpp>
#include <CL/cl_gl.h>

#include <vector>

/// <summary> The update location.</summary>
static const char* UPDATE_LOCATION = "kernels/updateFrame.cl";

///<summary> Class to do all the OpenCL calculations. </summary>
class CLHandler
{
public:

	friend Actor;

	/// <summary> Waits this object.</summary>
	static ENGINE_API void wait();


	 /// <summary> Initialises OpenCL </summary>
	 /// <param name='posBuffer'> Buffer for position data. </param>
	 /// <param	name='UVBuffer'> Buffer for uv data. </param>
	 /// <param	name='elemBuffer'> Buffer for element data.
	 /// <return> the error code </return>
	static ENGINE_API cl_int initCL(GLuint posBuffer, GLuint UVBuffer, GLuint elemBuffer);

	/// <summary> Updates the cl.</summary>
	///
	/// <param name="deltaTime"> The delta time.</param>
	/// <param name="data">		 [in,out] The data.</param>
	///
	/// <returns> A cl_int.</returns>
	static ENGINE_API cl_int updateCL(float deltaTime,  std::vector<ActorData>& data);

	/// <summary> Exit cl.</summary>
	static ENGINE_API void exitCL();

protected:
	/// <summary> The actors.</summary>
	static cl::Buffer actors;

	/// <summary> Gets the buffer for element cl data.</summary>
	///
	/// <value> A Buffer for element cl data.</value>
	static cl::BufferGL posCLBuffer, UVCLBuffer, elemCLBuffer;

	// pointer to the context -- needs to be a pointer or else it will freeze on exit . Will be null before initCL();
	static cl::Context* context;

	/// <summary> The platform.</summary>
	static cl::Platform platform;

	/// <summary> The queue.</summary>
	static cl::CommandQueue* queue;

	/// <summary> The devices.</summary>
	static std::vector<cl::Device> devices;

	/// <summary> The update program.</summary>
	static cl::Program* updateProgram;

	/// <summary> Gets the update kern.</summary>
	///
	/// <value> The update kern.</value>
	static cl::Kernel collideKern, updateKern;

	/// <summary> Loads cl program.</summary>
	///
	/// <param name="filepath"> The filepath.</param>
	/// <param name="program">  [in,out] [in,out] If non-null, the program.</param>
	///
	/// <returns> The cl program.</returns>
	static cl_int CLHandler::loadCLProgram(const GLchar* filepath, cl::Program*& program);

	/// <summary> Gets best platform.</summary>
	///
	/// <returns> The best platform.</returns>
	static cl::Platform getBestPlatform();
	
};