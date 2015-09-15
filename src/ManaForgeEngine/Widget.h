#pragma once
#include <deque>

class Widget
{
public:
	// Constructor takes start x and y, length and width, and optionally the subwidgets.
	Widget(int startX, int startY, int length, int height, const std::deque<Widget*>& subs = std::deque<Widget*>() );
	virtual ~Widget();

	// I don't know what draw will require, so I'll just not implement it yet.
	virtual void draw() const = 0;
private:
	int x;
	int y;
	int length;
	int height;
	std::deque<Widget*> subWidgets;
};