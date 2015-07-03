#include "EnginePCH.h"

#include "PropertyManager.h"

#include "Helper.h"

#include <boost/property_tree/json_parser.hpp>

PropertyManager::PropertyManager(const std::string& path)
{
	if (path != "")
	{
		init(path);
	}
}

void PropertyManager::init(const std::string& path)
{

	std::string a = loadFileToStr(path.c_str());

	std::ifstream stream{ path };



	try{

		boost::property_tree::json_parser::read_json(stream, props);
	}
	catch (std::exception& e)
	{
		logger<Error>() << e.what();
	}
}

