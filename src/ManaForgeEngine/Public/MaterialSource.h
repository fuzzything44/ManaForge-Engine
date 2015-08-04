#pragma once

#include "Engine.h"

class MaterialSource
{
  public:
	virtual ~MaterialSource() = default;

	virtual void init(const path_t& name) = 0;


	virtual path_t getName() const = 0;
};
