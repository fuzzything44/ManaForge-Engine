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
		boost::mpl::vector<CPosition, CVelocity, CAcceleration>
		, boost::mpl::vector0<>
		, boost::mpl::vector0<>
	>;

ENGINE_API extern std::shared_ptr<CoreManager_t> coreManager;

