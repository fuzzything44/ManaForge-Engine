#include "ENGException.h"

#include <sstream>

ENGException::ENGException(std::string reasonIn) 
{
	std::stringstream ss;

	ss << reasonIn << "\tStack: ";

	Stack s;
	s.ShowCallstack();

	reason = reasonIn + "\nStack: \n\n" + s();
}

const char* ENGException::what() const
{
	return reason.c_str();
}

