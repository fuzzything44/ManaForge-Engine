#pragma once

#include "Engine.h"

#include <vector>
#include <limits>

class Widget
{
public:
	inline explicit Widget(Widget* owner);
	inline Widget(Widget&& other);
	Widget(const Widget& other) = delete;

	inline virtual ~Widget();

	Widget& operator=(const Widget& other) = delete;

	inline virtual void draw(const mat3& drawMat){};

	inline void drawSubObjects(const mat3& drawmat);

	inline virtual void postDraw(const mat3& drawMat){};

	inline const Widget* getOwner() const { return owner; }
	inline Widget* getOwner() { return owner; }

	inline void setStartRelativeLocation(const vec2& newLoc) { locationStart = newLoc; }
	inline vec2 getStartRelativeLocation() const { return locationStart; }
	
	inline void setEndRelativeLocation(const vec2& newLoc) { locationEnd = newLoc; }
	inline vec2 getEndRelativeLocation() const { return locationEnd; }

protected:
	std::vector<Widget*> subWidgets;
	std::vector<Widget*>::size_type location;

	vec2 locationStart;
	vec2 locationEnd;

	Widget* const owner;
};

Widget::Widget(Widget* owner)
	: owner(owner)
{
	if (owner) {
		owner->subWidgets.push_back(this);
		location = owner->subWidgets.size() - 1;
	}
}

#ifdef max
#undef max
#endif

Widget::Widget(Widget&& other)
	: owner{other.owner}
	, location{other.location}
	, subWidgets(std::move(other.subWidgets))
	, locationStart{ other.locationStart }
	, locationEnd{ other.locationEnd }
{

	owner->subWidgets[other.location] = this;
	other.location = std::numeric_limits<std::vector<Widget*>::size_type>().max(); // change this to an invalid location
}

inline Widget::~Widget()
{
	if (owner) {
		auto&& lastElem = owner->subWidgets[owner->subWidgets.size() - 1]; // get the last element

		lastElem->location = location; // make it's location equal to our location.

		std::swap(owner->subWidgets[location], lastElem); // swap the elements

		owner->subWidgets.pop_back(); // then remove the last element
	}
}

inline void Widget::drawSubObjects(const mat3& drawMat)
{
	auto&& nextMat = glm::translate(drawMat, locationStart);
	for (auto&& elem : subWidgets) {
		elem->draw(nextMat);
		elem->drawSubObjects(nextMat); // recursively draw subobjects
		elem->postDraw(nextMat);
	}
}
