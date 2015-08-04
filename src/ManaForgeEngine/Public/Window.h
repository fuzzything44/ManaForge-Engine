#pragma once
#include <Engine.h>
#include <map>

#include <boost/core/noncopyable.hpp>

#include "WindowProps.h"

#include "KeyEnum.h"

/// <summary> A window.</summary>
class Window : boost::noncopyable
{
  public:
	virtual ~Window() = default;

	virtual const WindowProps& getWindowProps() const = 0;
	virtual void setWindowProps(const WindowProps& props) = 0;
	virtual void saveWindowProps() = 0;

	virtual int getIsKeyPressed(Keyboard key) = 0;
	virtual vec2 getCursorLocPixels() = 0;
};
