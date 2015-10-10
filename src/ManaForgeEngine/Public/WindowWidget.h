#pragma once
#include "Engine.h"

#include "Widget.h"

#include <map>
#include <type_traits>
namespace MFUI
{

	struct WindowProps;
	enum class Keyboard;

	/// <summary> A window.</summary>
	class WindowWidget : public Widget
	{
	public:
		/// <summary> Values that represent window modes.</summary>
		enum class WindowMode : uint8
		{
			FULLSCREEN = '0',			// Fullscreen - Steals the graphics card to render your game
			FULLSCREEN_WINDOWED = '1',	// Fullscreen windowed - Creates a borderless window that seems fullscren.
										//		Nice because it can be switched out very fast
			WINDOWED = '2'				// Windowed - Created a decorated window
		};

		/// <summary> Values that represent render modes.</summary>
		enum class RenderMode : uint8
		{
			NORMAL = 0,   // The normal render mode -- shaded polygons
			WIREFRAME = 1 // Render in wireframe
		};


		inline WindowWidget()
			: Widget(nullptr)
		{
		} // WindowWidget is top-level widget -- no owner.

		inline virtual WindowMode getWindowMode() = 0;

		inline virtual void setRenderMode(RenderMode newRenderMode) = 0;
		inline virtual RenderMode getRenderMode() const = 0;

		inline virtual void setTitle(std::string& newTitle) = 0;
		inline virtual std::string getTitle() const = 0;

		inline virtual void setSize(const ivec2& newSize) = 0;
		inline virtual ivec2 getSize() const = 0;

		inline virtual void setLocation(const ivec2& newLocation) = 0;
		inline virtual ivec2 getLocation() const = 0;

		inline virtual void setVisible(bool isVisible) = 0;
		inline virtual bool getVisible() const = 0;

		inline virtual void saveWindowProps() = 0;

		inline virtual int getIsKeyPressed(const Keyboard& key) = 0;
		inline virtual vec2 getCursorLocPixels() = 0;
	};

	// quick way to ostream the enum. Needed for serialization
	template <typename char_t, typename traits>
	inline std::basic_ostream<char_t, traits>& operator<<(std::basic_ostream<char_t, traits>& stream, WindowWidget::WindowMode mode)
	{
		stream << static_cast<std::underlying_type_t<WindowWidget::WindowMode>>(mode);
		return stream;
	}
	// and of course the istream
	template <typename char_t, typename traits>
	inline std::basic_istream<char_t, traits>& operator>>(std::basic_istream<char_t, traits>& stream, WindowWidget::WindowMode& mode)
	{
		std::underlying_type_t<WindowWidget::WindowMode> index;
		stream >> index;
		mode = static_cast<WindowWidget::WindowMode>(index);

		return stream;
	}


	template <typename char_t, typename traits>
	inline std::basic_ostream<char_t, traits>& operator<<(std::basic_ostream<char_t, traits>& stream, WindowWidget::RenderMode mode)
	{
		stream << static_cast<std::underlying_type_t<WindowWidget::RenderMode>>(mode);
		return stream;
	}

	template <typename char_t, typename traits>
	inline std::basic_istream<char_t, traits>& operator>>(std::basic_istream<char_t, traits>& stream, WindowWidget::RenderMode& mode)
	{
		std::underlying_type_t<WindowWidget::RenderMode> index;
		stream >> index;
		mode = static_cast<WindowWidget::RenderMode>(index);

		return stream;
	}

}// End namespace
#include "KeyEnum.h"