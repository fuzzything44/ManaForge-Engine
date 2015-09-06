#pragma once

#include <string>
#include <Widget.h>


namespace MFUI{
	class TextBox : public Widget
	{
	public:
		TextBox(int x, int y, std::string message);
		virtual ~TextBox();

		virtual void draw() const;

	private:
		std::string text;
	};

}