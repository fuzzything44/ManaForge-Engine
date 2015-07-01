#pragma once
#include "Engine.h"
#include "Logging.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>


class PropertyManager
{

public:

	ENGINE_API explicit PropertyManager(const std::string& location = "");
	ENGINE_API void init(const std::string& location);
	

	template <typename T>
	inline boost::optional<T> queryValue(const std::string& key);

	template <typename T>
	inline void saveValue(std::string key, const T& value);

private:
	boost::property_tree::ptree props;

};

template <typename T>
inline boost::optional<T> PropertyManager::queryValue(const std::string& key)
{
	try{
		return (props.get<T>(key));
	} catch (std::exception& e)
	{
		ENG_LOGLN(Error) << e.what();

		return boost::optional<T>(); // return "null"
	}
}

template <typename T>
inline void PropertyManager::saveValue(std::string key, const T& value)
{
	props.put(key, value);
}