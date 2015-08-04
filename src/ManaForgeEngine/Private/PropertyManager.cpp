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

	try
	{

		boost::property_tree::json_parser::read_json(stream, props);
	}
	catch (std::exception& e)
	{
		// pass it up the stack with a stack trace
		MFLOG(Error) << e.what();
	}
}
