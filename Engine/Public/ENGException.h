#pragma once
#include "Engine.h"
#include <Poppy/PoppyDebugTools.h>
#include <exception>
#include <string>

class ENGException : std::exception
{
public:
	virtual ENGINE_API const char* what() const override;

	ENGINE_API ENGException(std::string reasonIn, std::string stack);
	

private:
	std::string reason;
};


/// <summary> Called upon a fatal error.</summary>
#define FATAL_ERR(message, err) throw ENGException(message, Stack::GetTraceString())
