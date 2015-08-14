#pragma once
#include "Engine.h"

#include <boost/algorithm/clamp.hpp>

#include <boost/mpl/int.hpp>

template <typename T, typename min, typename max> class Clamp
{
public:
	static_assert(min::value <= max::value, "min must be less than or equal to max");

	Clamp()
		: data(min::value){};
	Clamp(T val)
		: data(boost::algorithm::clamp(val, min::value, max::value))
	{
	}

	inline T& get() { return data; }
	inline T get() const { return data; }

	operator T&() { return data; }
	operator T() const { return data; }

private:
	T data;
};

static constexpr float twoIntToFloat(float val, float dec)
{
	return dec < 1.f ? val + dec : twoIntToFloat(val, dec / 10.f);
}

template <int intVal, unsigned int decimal> struct float_
{
	static constexpr float value = twoIntToFloat((float)intVal, (float)decimal);
};

template <int minInt = 0, unsigned int minDec = 0, int maxInt = 1, unsigned int maxDec = 0>
using Clampf = Clamp<float, float_<minInt, minDec>, float_<maxInt, maxDec>>;

template <int min = 0, int max = 1> using Clampi = Clamp<int, boost::mpl::int_<min>, boost::mpl::int_<min>>;