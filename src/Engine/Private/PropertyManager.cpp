#include "EnginePCH.h"

#include "PropertyManager.h"

#include "Logging.h"
#include "Helper.h"

#include <boost/property_tree/json_parser.hpp>

PropertyManager::PropertyManager(const std::string& path)
{

	std::string a = loadFileToStr(path.c_str());

	std::ifstream stream{ path };



	try{

		boost::property_tree::json_parser::read_json(stream, props);
	} catch (std::exception& e)
	{
		ENG_LOGLN(Error) << e.what();
	}
}
