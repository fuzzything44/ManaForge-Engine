#include "EnginePCH.h"

#include "PropertyManager.h"

#include "Helper.h"

#include <boost/property_tree/json_parser.hpp>

#include <boost/filesystem/fstream.hpp>

PropertyManager::PropertyManager(const path_t& path)
{
	if (path != "") {
		init(path);
	}
}

void PropertyManager::init(const path_t& path)
{
	
	boost::filesystem::ifstream stream{path};
	
	boost::property_tree::json_parser::read_json(stream, props);
	
}
