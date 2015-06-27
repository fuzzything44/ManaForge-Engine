#pragma once

#include "Engine.h"
#include <string>

/// <summary> Values that represent window modes.</summary>
enum class WindowMode : uint8
{
	FULLSCREEN = 0,				// Fullscreen - Steals the graphics card to render your game
	FULLSCREEN_WINDOWED = 1,	// Fullscreen windowed - Creates a borderless window that seems fullscren
	WINDOWED = 2				// Windowed - Created a decorated window 
};

template<typename char_t, typename traits>
std::basic_ostream<char_t, traits>& operator<<(std::basic_ostream<char_t, traits>& stream, WindowMode mode)
{
	stream << static_cast<int>(mode);
	return stream;
}

template<typename char_t, typename traits>
std::basic_istream<char_t, traits>& operator>>(std::basic_istream<char_t, traits>& stream, WindowMode& mode)
{
	int index;
	stream >> index;
	mode = static_cast<WindowMode>(index);

	return stream;
}

/// <summary> Values that represent render modes.</summary>
enum class RenderMode : uint8
{
	NORMAL = 0,		// The normal render mode -- shaded polygons
	WIREFRAME = 1	// Render in wireframe
};

template<typename char_t, typename traits>
std::basic_ostream<char_t, traits>& operator<<(std::basic_ostream<char_t, traits>& stream, RenderMode mode)
{
	stream << static_cast<int>(mode);
	return stream;
}

template<typename char_t, typename traits>
std::basic_istream<char_t, traits>& operator>>(std::basic_istream<char_t, traits>& stream, RenderMode& mode)
{
	int index;
	stream >> index;
	mode = static_cast<RenderMode>(index);

	return stream;
}

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