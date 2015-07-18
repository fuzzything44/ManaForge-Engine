#pragma once

#include "Engine.h"

class MaterialSource
{
public:
	virtual ~MaterialSource() = default;

	virtual void init(const std::string& name) = 0;


	virtual std::string getName() = 0;

};

