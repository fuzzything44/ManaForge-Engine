#include "TextBox.h"

namespace MFUI {


	TextBox::TextBox(int x, int y, std::string message) : text(message), Widget(x, y, message.size() * 10, 10);
	{}

	void TextBox::draw() const
	{
		// Draws the text on the screen...
	}
}
