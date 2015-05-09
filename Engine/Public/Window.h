#pragma once
#include <Engine.h>
#include <map>

#include <boost/core/noncopyable.hpp>

#include "WindowProps.h"

#include "KeyEnum.h"

/// <summary> A window.</summary>
class Window :
	boost::noncopyable
{
public:

	Window() { }

	virtual ~Window()
	{
		
	}

	/// <summary> Sets render mode.</summary>
	///
	/// <param name="newMode"> The new mode.</param>
	virtual void setRenderMode(RenderMode newMode) = 0;

	/// <summary> Gets render mode.</summary>
	///
	/// <returns> The render mode.</returns>
	virtual RenderMode getRenderMode() = 0;

	/// <summary> initalizes the window.</summary>
	virtual void init() = 0;

	/// <summary> Gets the size.</summary>
	///
	/// <returns> The size.</returns>
	virtual uvec2 getSize() const = 0;

	virtual int32 getKey(Keyboard key) = 0;
	virtual vec2 getCursorLocPixels() = 0;


};

