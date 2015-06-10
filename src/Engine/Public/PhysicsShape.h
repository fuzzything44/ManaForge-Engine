#pragma once
#include "Engine.h"


enum class BodyType
{
	DYNAMIC,
	STATIC,
	KINEMATIC
};



class PhysicsShape
{
public:
	virtual ~PhysicsShape() { };

	virtual vec2 getPosition() = 0;
	virtual void setPosition(vec2 newPos) = 0;

	virtual void asCircle(float radius) = 0;
	virtual void asPolygon(vec2* verts, uint32 numVerts) = 0;
	virtual void asRectangle(float halfWidth, float halfHeight) = 0;
	virtual void asEdge(vec2 lineStart, vec2 lineEnd) = 0;
};
