#include "Box2DPhysicsShape.h"

Box2DPhysicsShape::~Box2DPhysicsShape() {}

vec2 Box2DPhysicsShape::getPosition() { return location; }

void Box2DPhysicsShape::setPosition(vec2 newPos) { location = newPos; }

void Box2DPhysicsShape::asCircle(float radius)
{
	std::unique_ptr<b2CircleShape> cir = std::make_unique<b2CircleShape>();
	cir->m_p = convertVec(location);
	cir->m_radius = radius;

	shape = std::move(cir);
}

void Box2DPhysicsShape::asPolygon(vec2* verts, uint32 numVerts)
{
	std::unique_ptr<b2PolygonShape> poly = std::make_unique<b2PolygonShape>();

	b2Vec2* vec = new b2Vec2[numVerts];

	// translate the points
	for (uint32 i = 0; i < numVerts; ++i)
	{
		vec[i] = convertVec(verts[i] + location);
	}

	poly->Set(vec, numVerts);

	shape = std::move(poly);
}

void Box2DPhysicsShape::asRectangle(float halfWidth, float halfHeight)
{
	std::unique_ptr<b2PolygonShape> box = std::make_unique<b2PolygonShape>();

	box->SetAsBox(halfWidth, halfHeight, b2Vec2(convertVec(location)), 0.f); // todo

	shape = std::move(box);
}

void Box2DPhysicsShape::asEdge(vec2 lineStart, vec2 lineEnd)
{
	std::unique_ptr<b2EdgeShape> edge = std::make_unique<b2EdgeShape>();

	edge->Set(convertVec(lineStart), convertVec(lineEnd));

	shape = std::move(edge);
}
