#pragma once
#include <Engine.h>
#include <map>

#include "Widget.h"


struct WindowProps;
enum class Keyboard;

/// <summary> A window.</summary>
class WindowWidget : public Widget
{
public:
	inline WindowWidget() : Widget(nullptr){} // Widget is top-level widget -- no owner.

	virtual const WindowProps& getWindowProps() const = 0;
	virtual void setWindowProps(const WindowProps& props) = 0;
	virtual void saveWindowProps() = 0;

	virtual int getIsKeyPressed(const Keyboard& key) = 0;
	virtual vec2 getCursorLocPixels() = 0;
};

#include "KeyEnum.h"
#include "WindowProps.h"