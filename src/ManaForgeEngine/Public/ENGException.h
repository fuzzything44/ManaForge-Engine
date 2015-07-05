﻿#pragma once
#include <exception>
#include <string>

#include "Engine.h"

class ENGException : std::exception
{
public:
	ENGINE_API virtual const char* what() const override;

	ENGINE_API explicit ENGException(const std::string& reasonIn = "");
	
};
