#pragma once
#include "Engine.h"
#include "Logging.h"

#include <exception>


class ENGException : std::exception
{
public:
	ENGINE_API virtual const char* what() const override;

	ENGINE_API explicit ENGException(std::string reasonIn);
	
};


/// <summary> Called upon a fatal error.</summary>
#define FATAL_ERR(message) \
	throw ENGException(message)\
	/**/
