#include "Box2DPhysicsShape.h"

Box2DPhysicsShape::~Box2DPhysicsShape()
{
	if (shape)
		delete shape;
}

vec2 Box2DPhysicsShape::getPosition()
{ 
	return location;
}

void Box2DPhysicsShape::setPosition(vec2 newPos)
{
	location = newPos;
}
	 
void Box2DPhysicsShape::asCircle(float radius)
{
	b2CircleShape* cir = new b2CircleShape();
	cir->m_p = convertVec(location);
	cir->m_radius = radius;
	
	shape = cir;
}

void Box2DPhysicsShape::asPolygon(vec2* verts, uint32 numVerts)
{
	b2PolygonShape* poly = new b2PolygonShape();

	b2Vec2* vec = new b2Vec2[numVerts];

	// translate the points
	for (int i = 0; i < numVerts; ++i)
	{
		vec[i] = convertVec(verts[i] + location);
	}

	poly->Set(vec, numVerts); 

	shape = poly;
	
}

void Box2DPhysicsShape::asRectangle(float halfWidth, float halfHeight)
{
	b2PolygonShape* box = new b2PolygonShape();

	box->SetAsBox(halfWidth, halfHeight, b2Vec2(convertVec(-location)), 0.f); // todo
	
	shape = box;
}

void Box2DPhysicsShape::asEdge(vec2 lineStart, vec2 lineEnd)
{
	b2EdgeShape* edge = new b2EdgeShape();

	edge->Set(convertVec(lineStart), convertVec(lineEnd));

	shape = edge;
}

