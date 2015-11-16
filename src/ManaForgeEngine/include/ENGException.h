#pragma once
#include <exception>
#include <string>

#include "Engine.h"

class ENGException : public std::exception
{
public:
	ENGINE_API virtual const char* what() const noexcept override;

	ENGINE_API explicit ENGException(const std::string& reasonIn = "");
	
	virtual ~ENGException() throw() { }
	
};
