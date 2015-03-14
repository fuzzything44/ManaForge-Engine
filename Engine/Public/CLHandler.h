#pragma once
#include "Engine.h"

#include "Helper.h"
#include "Logging.h"

class CLHandler;
#include "Actor.h"

#include <CL/cl.hpp>
#include <CL/cl_gl.h>

#include <vector>

static const char* UPDATE_LOCATION = "kernels/updateFrame.cl";

/**
 * \class	CLHandler
 *
 * \brief	Class to do all the OpenCL calculations.
 *
 * \author	Russell
 * \date	3/12/2015
 */

class CLHandler
{
public:

	friend Actor;

	/**
	 * \fn	static ENGINE_API cl_int CLHandler::initCL(GLuint posBuffer, GLuint UVBuffer, GLuint elemBuffer);
	 *
	 * \brief	Initialises the cl.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param	posBuffer 	Buffer for position data.
	 * \param	UVBuffer  	Buffer for uv data.
	 * \param	elemBuffer	Buffer for element data.
	 *
	 * \return	A cl_int.
	 */
	static ENGINE_API cl_int initCL(GLuint posBuffer, GLuint UVBuffer, GLuint elemBuffer);
	
	/**
	 * \fn	static ENGINE_API cl_int CLHandler::updateCL(vec2 characterLocation, std::vector<ActorData>& data);
	 *
	 * \brief	Updates CL and calls kernels to the data specified.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param	characterLocation	The character location.
	 * \param deltaTime             Delta Time
	 * \param [in,out]	data	 	The data.
	 *
	 * \return	A cl_int.
	 */
	static ENGINE_API cl_int updateCL(float deltaTime,  std::vector<ActorData>& data);

	/**
	 * \fn	static ENGINE_API void CLHandler::exitCL();
	 *
	 * \brief	Exit cl.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 */
	static ENGINE_API void exitCL();

protected:

	/** \brief	a buffer for the actorData. */
	static cl::Buffer actors;

	static cl::BufferGL posCLBuffer, UVCLBuffer, elemCLBuffer;

	// pointer to the context -- needs to be a pointer or else it will freeze on exit . Will be null before initCL();
	/** \brief	The context. */
	static cl::Context* context;

	/** \brief	the platform to use. Will be null before initCL() */
	static cl::Platform platform;

	/** \brief	the queue to use. Wil be invalid before initCL() */
	static cl::CommandQueue* queue;

	/** \brief	a list of the devices of platform. */
	static std::vector<cl::Device> devices;

	/** \brief	the program the contains the entire code for updating the frame. */
	static cl::Program* updateProgram;

	static cl::Kernel collideKern, updateKern;

	/**
	 * \fn	static cl_int CLHandler::loadCLProgram(const GLchar* filepath, cl::Program*& program);
	 *
	 * \brief	Loads cl program.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param	filepath	   	The filepath.
	 * \param [in,out]	program	[in,out] If non-null, the program.
	 *
	 * \return	The cl program.
	 */
	static cl_int CLHandler::loadCLProgram(const GLchar* filepath, cl::Program*& program);


	/**
	 * \fn	static cl::Platform CLHandler::getBestPlatform();
	 *
	 * \brief	Gets best platform.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \return	The best platform.
	 */
	static cl::Platform getBestPlatform();
	
};