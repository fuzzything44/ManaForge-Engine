#pragma once

#include "Engine.h"
#include "Manager.h"

struct CPosition
{
	vec2 value;
};

struct CVelocity
{
	vec2 value;
};

struct CAcceleration
{
	vec2 value;
};

using CoreManager_t =
	Manager
	<
		decltype(make_type_tuple<CPosition, CVelocity, CAcceleration>)
	>;

ENGINE_API extern CoreManager_t* coreManager;

