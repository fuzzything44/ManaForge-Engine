#include <stdafx.h>

#include "ENGException.h"

ENGException::ENGException(std::string reasonIn, std::string stack) 
{STACK
	std::stringstream ss;

	ss << reasonIn << "\tStack: " << stack;

	reason = ss.str();
}

const char* ENGException::what() const
{STACK
	return reason.c_str();
}

