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
	material(static_cast<OpenGLMaterial*>(data.material))
{	
	// make sure they aren't zero
	check(numVerts);
	check(numTris);

	// allocate data
	locations = static_cast<vec2*>(malloc(sizeof(vec2) * numVerts));
	UVs = static_cast<vec2*>(malloc(sizeof(vec2) * numVerts));
	elems = static_cast<uint32*>(malloc(sizeof(uvec3) * numTris));

	// copy data over
	memcpy(locations, data.vertexLocations, numVerts * sizeof(vec2));
	memcpy(UVs, data.UVs, numVerts * sizeof(vec2));
	memcpy(elems, data.triangles, numTris * sizeof(uvec3));

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * numTris, elems, GL_STATIC_DRAW);


	// get the cameraMat location
	check(material);
	viewMatUniformLocation = glGetUniformLocation((*material)(), "MVPmat");
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

	free(locations);
	free(UVs);
	free(elems);

	renderer->models.remove(this);
}

void OpenGLModel::draw()
{

	material->use();

	glBindVertexArray(vertexArray);

	Transform worldTrans = trans + parent->getWorldTransform();

	mat4 camera = (renderer->getCurrentCamera().getViewMat());
	mat4 translation = glm::translate(vec3(worldTrans.location.x, worldTrans.location.y, 0.f));
	mat4 rotation = glm::rotate(worldTrans.rotation, vec3(0.f, 0.f, 1.f));
	mat4 scale = glm::scale(vec3(worldTrans.scale.x, worldTrans.scale.y, 1.f));
	
	mat4 model = translation * rotation * scale;

	mat4 MVP = model * camera;

	// set the viewMat in the shader to the view mat defined by the camera that is current
	glUniformMatrix4fv(viewMatUniformLocation, 1, GL_FALSE, &MVP[0][0]);
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
		numTris, // these many verticies
		GL_UNSIGNED_INT, // the data is uint32 - unsigned int
		nullptr // use the buffer instead of raw data
		);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
