#include "OpenGLRendererConfig.h"

#include <OpenGLRenderer.h>
#include <Transform.h>
#include <ModuleManager.h>
#include <Helper.h>
#include <CameraComponent.h>
#include <MeshComponent.h>

#include <glm-ortho-2d.h>

OpenGLModel::OpenGLModel(ModelData data, MeshComponent* owner, OpenGLRenderer* renderer)
	:numVerts(data.numVerts),
	numTris(data.numTriangles),
	renderer(renderer),
	parent(owner),
	bounds(data.bounds),
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
	MVPUniformLocation = glGetUniformLocation((*material)(), "MVPmat");
}


OpenGLModel::~OpenGLModel()
{

	glDeleteBuffers(1, &vertexLocationBuffer);
	glDeleteBuffers(1, &texCoordBuffer);
	glDeleteBuffers(1, &elemBuffer);

	glDeleteVertexArrays(1, &vertexArray);

	if (!OpenGLRenderer::isDestroying)
	{
		check(renderer);

		renderer->models.remove(this);
	}
}

MeshComponent* OpenGLModel::getOwnerComponent()
{
	return parent;
}

bool OpenGLModel::isInBounds(const mat3& model, const mat4& camera) // TODO: ACTUAL COLLUISION
{
	
	vec3 locBeforeCamera = model * vec3(bounds.lowerLeft, 1.f);
	vec4 posLowerLeft = camera * vec4(locBeforeCamera.x, locBeforeCamera.y, -1.f, 1.f);

	locBeforeCamera = model * vec3(bounds.upperLeft, 1.f);
	vec4 posUpperLeft = camera * vec4(locBeforeCamera.x, locBeforeCamera.y, -1.f, 1.f);

	locBeforeCamera = model * vec3(bounds.lowerRight, 1.f);
	vec4 posLowerRight = camera * vec4(locBeforeCamera.x, locBeforeCamera.y, -1.f, 1.f);

	locBeforeCamera = model * vec3(bounds.upperRight, 1.f);
	vec4 posUpperRight = camera * vec4(locBeforeCamera.x, locBeforeCamera.y, -1.f, 1.f);
	

	return true; // for now
}

void OpenGLModel::draw()
{
	check(parent);
	
	//// start with the camera
	//mat3 MVPmat = renderer->getCurrentCamera().getViewMat();

	//MVPmat = glm::scale(MVPmat, worldTrans.scale);			// apply scaling
	//MVPmat = glm::rotate(MVPmat, worldTrans.rotation);		// apply rotation
	//MVPmat = glm::translate(MVPmat, worldTrans.location);	// apply translation

	mat3 view = renderer->getCurrentCamera().getViewMat();
	mat3 model = parent->getModelMatrix();

	mat3 MVPmat = view * model;

	// check if it is in bounds
	//if (!isInBounds(model, camera))
	//	return;

	material->use();

	glBindVertexArray(vertexArray);


	if (MVPUniformLocation != -1)
	{
		glUniformMatrix3fv(MVPUniformLocation, 1, GL_FALSE, &MVPmat[0][0]);
	}
	else
	{
		FATAL_ERR("could not find MVPmat uniform in shader");
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
