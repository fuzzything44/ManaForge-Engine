#include "TextBox.h"

using std::string;

namespace MFUI {

	// Sets text to message and calls super with given starting position. Height is the height of a character in pixels
	//     , length is the amount of characters * length of a character in pixels. A text box has no sub-widgets.
	TextBox::TextBox(int x, int y, const string& message) 
		: text(message), Widget(x, y, message.size() * 10, 10) {}

	void TextBox::setText(const string& newMessage)
	{
		text = newMessage;
	}

	string TextBox::getText()
	{
		return text;
	}

	void TextBox::draw() const
	{
		// Draws the text on the screen...
	}
}
