#include "Widget.h"

Widget::Widget(int startX, int startY, int length, int width, const std::deque<Widget>& subs = std::deque<Widget>()) 
	: x(startX), y(startY), length(length), width(width), subWidgets(subs) {}

Widget::~Widget() {}
