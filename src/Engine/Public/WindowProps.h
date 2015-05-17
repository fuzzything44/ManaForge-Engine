#pragma once

#include "Engine.h"
#include <string>

/// <summary> Values that represent window modes.</summary>
enum class WindowMode : uint8
{
	FULLSCREEN,				// Fullscreen - Steals the graphics card to render your game
	FULLSCREEN_WINDOWED,	// Fullscreen windowed - Creates a borderless window that seems fullscren
	WINDOWED				// Windowed - Created a decorated window 
};

/// <summary> Values that represent render modes.</summary>
enum class RenderMode : uint8
{
	NORMAL,		// The normal render mode -- shaded polygons
	WIREFRAME	// Render in wireframe
};


// the propertys that a window has
struct WindowProps
{
	explicit WindowProps(
		RenderMode renderMode = RenderMode::NORMAL,
		WindowMode windowMode = WindowMode::WINDOWED,
		const std::string& title = "",
		const uvec2& size = uvec2(100.f, 100.f),
		bool visible = true)
		:renderMode(renderMode),
		windowMode(windowMode),
		title(title),
		size(size),
		visible(visible)
		{ }

	RenderMode renderMode;
	WindowMode windowMode;
	std::string title;
	uvec2 size;
	bool visible;
};