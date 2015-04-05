#include <stdafx.h>

#include "ENGException.h"

ENGException::ENGException(std::string reasonIn, int err, char* file, int line) 
{
	std::stringstream ss;

	ss << reasonIn << "\tIn file: " << file << " : " << line << "\tError Number: " << err;

	reason = ss.str();
}

const char* ENGException::what() const
{
	return reason.c_str();
}

