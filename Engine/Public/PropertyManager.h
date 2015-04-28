#pragma once

#include "Engine.h"
#include "Logging.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>

class PropertyManager
{

public:

	ENGINE_API explicit PropertyManager(std::string location);

	template <typename T>
	inline T queryValue(std::string key);

	ENGINE_API const boost::property_tree::ptree& getSubTree(std::string path);

private:
	boost::property_tree::ptree props;

};

template <typename T>
inline T PropertyManager::queryValue(std::string key)
{
	try{
		return (props.get<T>(key));
	} catch (std::exception& e)
	{
		ENG_LOG(e.what());

		return T();
	}
}
