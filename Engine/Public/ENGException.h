#pragma once

#include "Engine.h"
#include <exception>
#include <string>

class ENGException : std::exception
{
public:
	virtual const char* what() const override;

	ENGException(std::string reasonIn, int err, char* file, int line);
	

private:
	std::string reason;
};
