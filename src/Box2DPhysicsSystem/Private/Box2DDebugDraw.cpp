#include "Box2DDebugDraw.h"

#include <Color.h>
#include <Runtime.h>
#include <Renderer.h>

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	auto verts = std::vector<vec2>(vertexCount);
	for (int i = 0; i < vertexCount; ++i)
	{
		verts[i] = convertVec(vertices[i]);
	}
	Color col = Color(uint8(color.r * 255.f), uint8(color.g * 255.f), uint8(color.b * 255.f), 255);

	Runtime::get().renderer->drawDebugOutlinePolygon(verts.data(), vertexCount, col);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	auto verts = std::vector<vec2>(vertexCount);
	for (int i = 0; i < vertexCount; ++i)
	{
		verts[i] = convertVec(vertices[i]);
	}
	Color col = Color(uint8(color.r * 255.f), uint8(color.g * 255.f), uint8(color.b * 255.f), 255);

	Runtime::get().renderer->drawDebugSolidPolygon(verts.data(), vertexCount, col);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	Runtime::get().renderer->drawDebugOutlineCircle(
	    convertVec(center), radius, Color(uint8(color.r * 255.f), uint8(color.g * 255.f), uint8(color.b * 255.f), 255));
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	Runtime::get().renderer->drawDebugSolidCircle(
	    convertVec(center), radius, Color(uint8(color.r * 255.f), uint8(color.g * 255.f), uint8(color.b * 255.f), 255));
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	Runtime::get().renderer->drawDebugSegment(
	    convertVec(p1), convertVec(p2), Color(uint8(color.r * 255.f), uint8(color.g * 255.f), uint8(color.b * 255.f), 255));
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
	// b2Vec2 p1 = xf.p, p2;
	// const float32 k_axisScale = 0.4f;
	// glBegin(GL_LINES);

	// glColor3f(1.0f, 0.0f, 0.0f);
	// glVertex2f(p1.x, p1.y);
	// p2 = p1 + k_axisScale * xf.q.GetXAxis();
	// glVertex2f(p2.x, p2.y);

	// glColor3f(0.0f, 1.0f, 0.0f);
	// glVertex2f(p1.x, p1.y);
	// p2 = p1 + k_axisScale * xf.q.GetYAxis();
	// glVertex2f(p2.x, p2.y);

	// glEnd();
}

void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	// glPointSize(size);
	// glBegin(GL_POINTS);
	// glColor3f(color.r, color.g, color.b);
	// glVertex2f(p.x, p.y);
	// glEnd();
	// glPointSize(1.0f);
}

void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{
	auto points =
	    std::array<vec2, 4>{{vec2{aabb->lowerBound.x, aabb->lowerBound.y}, vec2{aabb->upperBound.x, aabb->lowerBound.y},
	                         vec2{aabb->upperBound.x, aabb->upperBound.y}, vec2{aabb->lowerBound.x, aabb->upperBound.y}}};


	Runtime::get().renderer->drawDebugOutlinePolygon(points.data(), 4,
	                                                 Color(uint8(c.r * 255.f), uint8(c.g * 255.f), uint8(c.b * 255.f), 255));
}
