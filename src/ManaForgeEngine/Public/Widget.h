#pragma once

#include "Engine.h"
#include "Transform.h"

#include <vector>

class Widget
{
public:

	virtual void draw() { };

	inline void drawSubObjects();

protected:

	std::vector<Widget*> subWidgets;

	Transform trans;

};

inline void Widget::drawSubObjects()
{
	std::for_each(subWidgets.begin(), subWidgets.end(), draw);
}
