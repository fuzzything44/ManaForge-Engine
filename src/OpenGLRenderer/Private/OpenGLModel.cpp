#include "OpenGLRendererConfig.h"

#include <OpenGLRenderer.h>
#include <Transform.h>
#include <ModuleManager.h>
#include <Helper.h>
#include <CameraComponent.h>
#include <MeshComponent.h>


OpenGLModel::OpenGLModel(ModelData data, MeshComponent* owner, OpenGLRenderer* renderer)
	:numVerts(data.numVerts),
	numTris(data.numTriangles),
	renderer(renderer),
	parent(owner),
	trans(data.trans),
	material(static_cast<OpenGLMaterial*>(data.material))
{	
	// make sure they aren't zero
	check(numVerts);
	check(numTris);

	// add model to renderer's list
	renderer->models.push_front(this);

	// init GL buffers
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	// init location buffer
	glGenBuffers(1, &vertexLocationBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexLocationBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * numVerts, data.vertexLocations, GL_STATIC_DRAW);

	// init UV buffer
	glGenBuffers(1, &texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * numVerts, data.UVs, GL_STATIC_DRAW);

	// init elem buffer
	glGenBuffers(1, &elemBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uvec3) * numTris, data.triangles, GL_STATIC_DRAW);


	// get the cameraMat location
	check(material);
	cameraUniformLocation = glGetUniformLocation((*material)(), "camera");
	modelUniformLocation = glGetUniformLocation((*material)(), "model");
}



Transform OpenGLModel::getTransform() const
{
	return trans;
}

vec2 OpenGLModel::getLocation() const
{
	return trans.location;
}

float OpenGLModel::getRotation() const
{
	return trans.rotation;
}

vec2 OpenGLModel::getScale() const
{
	return trans.scale;
}

void OpenGLModel::setTransform(Transform newTransform)
{
	trans = newTransform;
}


void OpenGLModel::setLocation(vec2 newLocation)
{
	trans.location = newLocation;
}

void OpenGLModel::setRotation(float newRotation)
{
	trans.rotation = newRotation;
}

void OpenGLModel::setScale(vec2 newScale)
{
	trans.scale = newScale;
}

MeshComponent* OpenGLModel::getOwnerComponent()
{
	return parent;
}

void OpenGLModel::addRelativeTransform(Transform add)
{
	trans += add;
}

void OpenGLModel::addRelativeLocation(vec2 locToAdd)
{
	trans.location += locToAdd;
}

void OpenGLModel::addRelativeRotation(float rotToAdd)
{
	trans.rotation += rotToAdd;
}

void OpenGLModel::addRelativeScale(vec2 scaleToAdd)
{
	trans.scale += scaleToAdd;
}

OpenGLModel::~OpenGLModel()
{
	check(renderer);

	glDeleteBuffers(1, &vertexLocationBuffer);
	glDeleteBuffers(1, &texCoordBuffer);
	glDeleteBuffers(1, &elemBuffer);

	glDeleteVertexArrays(1, &vertexArray);

	renderer->models.remove(this);
}

void OpenGLModel::draw()
{

	material->use();

	glBindVertexArray(vertexArray);



	Transform worldTrans;

	if (parent)
	{
		worldTrans = trans + parent->getWorldTransform(); // todo fix this
	}
	else
	{
		worldTrans = trans;
	}

	mat4 camera = (renderer->getCurrentCamera().getViewMat());

	// create an identity matrix
	mat3 model{1.f};

	model = glm::rotate(model, worldTrans.rotation);		// apply rotation
	model = glm::scale(model, worldTrans.scale);			// apply scaling
	model = glm::translate(model, worldTrans.location);	// apply translation


	if (cameraUniformLocation != -1)
	{
		// set the camera matrix in the shader to the view mat defined by the camera that is current
		glUniformMatrix4fv(
			cameraUniformLocation,	// location of the uniform
			1,						// only one matrix
			GL_FALSE,				// don't transpose it
			&camera[0][0]);			// pointer to the first element
		
	}
	else
	{
		FATAL_ERR("could not find camera uniform in shader");
	}

	if (modelUniformLocation != -1)
	{
		glUniformMatrix3fv(modelUniformLocation, 1, GL_FALSE, &model[0][0]);
	}
	else
	{
		FATAL_ERR("could not find model uniform in shader");
	}


	glUniform1f(glGetUniformLocation((*material)(), "renderOrder"), 1.f);

	// bind location data to the element attrib array so it shows up in our shaders -- the location is zero (look in shader)
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexLocationBuffer);
	glVertexAttribPointer(
		0, // location 0 (see shader)
		2, // two elements per vertex (x,y)
		GL_FLOAT, // they are floats
		GL_FALSE, // not normalized 
		sizeof(float) * 2, // the next element is 2 floats later
		nullptr // dont copy -- use the GL_ARRAY_BUFFER instead
		);

	// bind UV data to the element attrib array so it shows up in our sahders -- the location is  (look in shader)
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glVertexAttribPointer(
		1, // location 1 (see shader)
		2, // two elements per vertex (u,v)
		GL_FLOAT, // they are floats
		GL_FALSE, // not normalized 
		sizeof(float) * 2, // the next element is 2 floats later
		nullptr // use the GL_ARRAY_BUFFER instead of copying on the spot
		);

	// bind the element buffer so it is used to make our draw call
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer);

	glDrawElements(
		GL_TRIANGLES, // they are trianges
		numTris * 3, // these many trainges - three verts / triangle
		GL_UNSIGNED_INT, // the data is uint32 - unsigned int
		nullptr // use the buffer instead of raw data
		);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
