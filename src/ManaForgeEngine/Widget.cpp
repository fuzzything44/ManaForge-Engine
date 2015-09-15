#include "Widget.h"

Widget::Widget(int startX, int startY, int length, int height, const std::deque<Widget>& subs = std::deque<Widget>()) 
	: x(startX), y(startY), length(length), height(height), subWidgets(subs) {}

Widget::~Widget() {}