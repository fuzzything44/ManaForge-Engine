#pragma once
#include "Engine.h"

#include "Logging.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>


class PropertyManager
{

public:

	ENGINE_API explicit PropertyManager(const std::string& location);

	template <typename T>
	inline T queryValue(const std::string& key);

	template <typename T>
	inline void saveValue(std::string key, const T& value);

private:
	boost::property_tree::ptree props;

};

template <typename T>
inline T PropertyManager::queryValue(const std::string& key)
{
	try{
		return (props.get<T>(key));
	} catch (std::exception& e)
	{
		ENG_LOGLN(e.what());

		return T();
	}
}

template <typename T>
inline void PropertyManager::saveValue(std::string key, const T& value)
{
	props.put(key, value);
}