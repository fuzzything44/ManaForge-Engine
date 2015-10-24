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

