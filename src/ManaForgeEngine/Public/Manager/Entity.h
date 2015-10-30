#pragma once

#include <limits>

#ifdef max
#undef max
#endif

// Basic entity data
template<typename ManagerType>
struct Entity
{
	friend ManagerType;

	explicit Entity(size_t GUID_ = std::numeric_limits<size_t>::max())
		:// components{ components_ }
		GUID{ GUID_ } {}

	size_t GUID;


	bool matchesSignature(typename ManagerType::RuntimeSignature_t toTest)
	{
		return (components & toTest) == toTest;

	}

	bool isValid()
	{
		return GUID != std::numeric_limits<size_t>::max();
	}


	typename ManagerType::RuntimeSignature_t components;

};

