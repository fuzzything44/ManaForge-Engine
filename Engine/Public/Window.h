#pragma once
#include <Engine.h>
#include <map>


/// <summary> Values that represent window modes.</summary>
enum WindowMode
{
	FULLSCREEN,				// Fullscreen - Steals the graphics card to render your game
	FULLSCREEN_WINDOWED,	// Fullscreen windowed - Creates a borderless window that seems fullscren
	WINDOWED				// Windowed - Created a decorated window 
};

/// <summary> Values that represent render modes.</summary>
enum RenderMode
{
	NORMAL,		// The normal render mode -- shaded polygons
	WIREFRAME	// Render in wireframe
};

/// <summary> A window.</summary>
class Window
{
public:

	/// <summary> Sets render mode.</summary>
	///
	/// <param name="newMode"> The new mode.</param>
	ENGINE_API void setRenderMode(RenderMode newMode);

	/// <summary> Gets render mode.</summary>
	///
	/// <returns> The render mode.</returns>
	ENGINE_API RenderMode getRenderMode();

	/// <summary> Windows.</summary>
	///
	/// <param name="title"> The title.</param>
	/// <param name="mode">  The mode.</param>
	/// <param name="size">  The size.</param>
	///
	/// <returns> An ENGINE_API.</returns>
	ENGINE_API Window(std::string title, WindowMode mode, uvec2 size);

	/// <summary> Runs the window. Consumes the thread.</summary>
	ENGINE_API void run();

	/// <summary> Gets the size.</summary>
	///
	/// <returns> The size.</returns>
	ENGINE_API uvec2 getSize() const;

	ENGINE_API int32 getKey(int key);
	ENGINE_API vec2 getCursorLoc();

protected:

	/// <summary> The title.</summary>
	std::string title;
	/// <summary> The mode.</summary>
	WindowMode mode;

	/// <summary> Updates the window parameters.</summary>
	ENGINE_API void updateWindowParameters();
	 
	/// <summary> true if this object has focus.</summary>
	bool hasFocus;
	
	/// <summary> The window.</summary>
	GLFWwindow* window;

	/// <summary> The render mode.</summary>
	RenderMode renderMode;


	/// <summary> Called upon initalization. Useful for creating graphics buffers and onscreen objects.</summary>
	ENGINE_API virtual void init() = 0;

	/// <summary> Called when the window is scrolled.</summary>
	///
	/// <param name="x"> The amount in x scroll.</param>
	/// <param name="y"> The y coordinate.</param>
	ENGINE_API virtual void scroll(float x, float y) = 0;

	/// <summary> Called every frame after input.</summary>
	///
	/// <param name="deltaTime"> The delta time since last frame.</param>
	ENGINE_API virtual void draw(float deltaTime) = 0;

	/// <summary> Called every frame before draw.</summary>
	///
	/// <param name="deltaTime"> The delta time since last frame.</param>
	ENGINE_API virtual void input(float deltaTime) = 0;

	/// <summary> Called when the window is destroyed.</summary>
	ENGINE_API virtual void exit() = 0;

	/// <summary> Focus.</summary>
	///
	/// <param name="focused"> The focused.</param>
	ENGINE_API virtual void focus(int32 focused) = 0;

	/// <summary> All windows sorted by the GLFWwindow pointer.</summary>
	ENGINE_API static std::map<GLFWwindow*, Window*> windows;

	/// <summary> Callback, called on scroll.</summary>
	///
	/// <param name="window"> [in,out] If non-null, the window.</param>
	/// <param name="x">	  The x coordinate.</param>
	/// <param name="y">	  The y coordinate.</param>
	ENGINE_API static void scrollCallback(GLFWwindow* window, double x, double y)
	{STACK
		if (Window* winObj = windows[window])
		{
			winObj->scroll((float)x, (float)y);
		}
	}

	/// <summary> Callback, called when the window receives focus.</summary>
	///
	/// <param name="window">  [in,out] If non-null, the window.</param>
	/// <param name="focused"> The focused.</param>
	ENGINE_API static void focusCallback(GLFWwindow* window, int focused)
	{STACK
		if (Window* winObj = windows[window])
		{
			winObj->focus(focused);
		}
	}
};

