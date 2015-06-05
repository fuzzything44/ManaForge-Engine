#pragma once

#include <Engine.h>
#include <Model.h>
#include "OpenGLMaterial.h"

class OpenGLModel : public Model
{
public:

	friend class OpenGLRenderer;

	explicit OpenGLModel(ModelData data, MeshComponent* owner, OpenGLRenderer* renderer);

	MeshComponent* getOwnerComponent() override;

	virtual ~OpenGLModel();

protected:

	void draw();

	bool isInBounds(const mat3& model, const mat4& camera);

	uint32 numVerts;
	uint32 numTris;

	uint32 vertexArray;
	uint32 vertexLocationBuffer;
	uint32 texCoordBuffer;
	uint32 elemBuffer;

	GLint MVPUniformLocation;

	ModelBounds bounds;

	MeshComponent* parent;
	OpenGLRenderer* renderer;

	OpenGLMaterial* material;

};