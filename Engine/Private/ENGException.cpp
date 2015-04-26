#include "ENGException.h"

ENGException::ENGException(std::string reasonIn, std::string stack) 
{
	std::stringstream ss;

	ss << reasonIn << "\tStack: " << stack;

	reason = ss.str();
}

const char* ENGException::what() const
{
	return reason.c_str();
}

