#include <stdafx.h>

#include "ENGException.h"

ENGException::ENGException(std::string reasonIn, std::string stack, int err, char* file, int line) 
{STACK
	std::stringstream ss;

	ss << reasonIn << "\tIn file: " << file << " : " << line << "\tError Number: " << err << "\n\nStack: " << stack;

	reason = ss.str();
}

const char* ENGException::what() const
{STACK
	return reason.c_str();
}

