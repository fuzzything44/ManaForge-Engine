#pragma once
#include "Box2DPhysicsSystemConfig.h"

#include <PhysicsShape.h>

class Box2DPhysicsShape : public PhysicsShape
{
	friend class Box2DPhysicsBody;

public:
	virtual ~Box2DPhysicsShape();

	virtual vec2 getPosition() override;
	virtual void setPosition(vec2 newPos) override;

	virtual void asCircle(float radius) override;
	virtual void asPolygon(vec2* verts, uint32 numVerts) override;
	virtual void asRectangle(float halfWidth, float halfHeight) override;
	virtual void asEdge(vec2 lineStart, vec2 lineEnd) override;

private:
	vec2 location;
	std::unique_ptr<b2Shape> shape;
};
