#include "stdafx.h"
#include "Actor.h"

///////////// STATIC VARIBALE INITALIZATION ////////////////////

GLint Actor::texUniformHandle = -1;
GLint Actor::viewMatUniformHandle = -1;

GLuint Actor::vertexArray = 0;
GLuint Actor::locBuffer = 0;
GLuint Actor::UVBuffer = 0;
GLuint Actor::elemBuffer = 0;

GLuint Actor::program = 0;

mat4* Actor::viewMat = 0;

Actor::Actor(const ActorData& dataIn, Chunk* chunkIn) 
	: data(dataIn),
	chunk(chunkIn)
{
	// set the pointer to this class
	data.actorPtr = this;

	// add this class to the chunk
	chunk->actors.push_back(this);

	ENG_LOG("\nCreate Buffer For Actor" << std::endl);

//	cl_int err;
	//buff = cl::Buffer(*CLHandler::context, CL_MEM_READ_WRITE, sizeof(ActorData), &data, &err);

//	errChkCL(err)

}

GLint Actor::init(GLuint programIn, mat4* viewMatIn)
{
	program = programIn;

	viewMat = viewMatIn;



	// create a massive array of zeros for default value
	float* empty = (float*)malloc(sizeof(float) * 2 * 4 * 1000);

	memset(empty, 0, sizeof(float) * 2 * 4 * 1000);

	// set uniform location 
	texUniformHandle = glGetUniformLocation(program, "tex");
	viewMatUniformHandle = glGetUniformLocation(program, "viewMat");


	// init actor buffers
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);


	glGenBuffers(1, &locBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, locBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 4 * 100, (void*)empty, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &UVBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 4 * 100, (void*)empty, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &elemBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6 * 100, (void*)empty, GL_DYNAMIC_DRAW);


	free(empty);

	CLHandler::initCL(locBuffer, UVBuffer, elemBuffer);


	return EXIT_SUCCESS;
}


void Actor::drawActors(std::vector<ActorData>& data, float deltaTime, vec2 characterLoc)
{

	CLHandler::updateCL(characterLoc, deltaTime, data);

	// use the correct program
	glUseProgram(program);

	// set the program to use texture unit 0
	glUniform1i(texUniformHandle, 0);

	// bind texture unit 0 to our texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureLibrary::getTextureHandle());


	// set the view matrix
	if (viewMatUniformHandle != -1)
	{
		glUniformMatrix4fv(viewMatUniformHandle, 1, GL_FALSE, &(*viewMat)[0][0]);
	}

	glBindVertexArray(vertexArray);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, locBuffer);
	glVertexAttribPointer(
		0, // location 0 (see shader)
		3, // three elements per vertex (x,y,z)
		GL_FLOAT, // they are floats
		GL_FALSE, // not normalized (look up vector normalization)
		sizeof(GLfloat) * 3, // the next element is 3 floats later
		0 // dont copy -- use the GL_ARRAY_BUFFER instead
		);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
	glVertexAttribPointer(
		1, // location 1 (see shader)
		2, // two elements per vertex (x,y)
		GL_FLOAT, // they are floats
		GL_FALSE, // not normalized (look up vector normalization)
		sizeof(GLfloat) * 2, // the next element is 2 floats later
		0 // dont copy -- use the GL_ARRAY_BUFFER instead
		);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer);

	glDrawElements(GL_TRIANGLES, data.size() * 6, GL_UNSIGNED_INT, 0);

	// disable vertex pointers 
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}



void Actor::tick(float deltaTime)
{
	needsUpdate = false;
}

vec2 Actor::getLocation()
{
	return data.location;
}

vec2  Actor::getSize()
{
	return data.size;
}

float Actor::getRotation()
{
	return data.rotation;
}

vec2  Actor::getVelocity()
{
	return data.velocity;
}

void Actor::setLocation(vec2 newLoc)
{
	data.location = newLoc;
	needsUpdate = true;
}

void Actor::setSize(vec2 newSize)
{
	data.size = newSize;
	needsUpdate = true;
}

void Actor::setRotation(float newRot)
{
	data.rotation = newRot;
	needsUpdate = true;
}

void Actor::setVelocity(vec2 newVelocity)
{
	data.velocity = newVelocity;
	needsUpdate = true;
}
