#pragma once

#include "Engine.h"
#include "Transform.h"

#include <vector>

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

protected:
	std::vector<Widget*> subWidgets;
	std::vector<Widget*>::size_type location;

	// the relative transform
	Transform trans;

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

Widget::Widget(Widget&& other)
	: owner{other.owner}
	, location{other.location}
	, subWidgets(std::move(other.subWidgets))
	, trans{other.trans}
{
	owner->subWidgets[other.location] = this;
	other.location = MAXSIZE_T; // change this to an invalid location
}

inline Widget::~Widget()
{
	if (owner)
	{
		auto&& lastElem = owner->subWidgets[owner->subWidgets.size() - 1]; // get the last element

		lastElem->location = location; // make it's location equal to our location.

		std::swap(owner->subWidgets[location], lastElem); // swap the elements

		owner->subWidgets.pop_back(); // then remove the last element
	}

}

inline void Widget::drawSubObjects(const mat3& drawMat)
{
	auto&& nextMat = glm::translate(drawMat, trans.location);
	for (auto&& elem : subWidgets) {
		elem->draw(nextMat);
		elem->drawSubObjects(nextMat); // recursively draw subobjects
	}
}
