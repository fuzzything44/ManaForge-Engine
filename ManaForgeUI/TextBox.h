#pragma once

#include <string>
#include <Widget.h>


namespace MFUI{
	class TextBox : public Widget
	{
	public:
		TextBox(int x, int y, const std::string& message);
		virtual ~TextBox();

		// Allows getting and setting of text.
		virtual void setText(const std::string& newMessage);
		virtual std::string getText();

		virtual void draw() const;

	private:
		std::string text;
	};

}