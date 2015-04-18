#include <OpenGLRenderer.h>
#include <Transform.h>
#include <ModuleManager.h>
#include <Helper.h>

OpenGLModel::OpenGLModel(vec2* locations, vec2* UVs, uint32* elems, uint32 numVerts, uint32 numElems, OpenGLRenderer* renderer)
	:locations(locations),
	UVs(UVs),
	elems(elems),
	numVerts(numVerts),
	numElems(numElems),
	renderer(renderer)
{
	// add model to renderer's list
	renderer->models.push_front(this);

	// init GL buffers
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	// init location buffer
	glGenBuffers(1, &vertexLocationBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexLocationBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * numVerts, locations, GL_STATIC_DRAW);

	// init UV buffer
	glGenBuffers(1, &texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * numVerts, locations, GL_STATIC_DRAW);

	// init elem buffer
	glGenBuffers(1, &elemBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * numElems, elems, GL_STATIC_DRAW);

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

	renderer->models.remove(this);
}

void OpenGLModel::draw()
{

}