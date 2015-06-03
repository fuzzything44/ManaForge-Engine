#pragma once

#include "Engine.h"
#include "Logging.h"
#include "ENGException.h"
#include "Helper.h"

#include <vector>
#include <string>
#include <map>
#include <functional>

class ImageLoader
{
public:
	typedef std::function<uvec2(std::string, std::vector<uint8>&)> loadFun;
	
	ENGINE_API static void addLoader(std::string extension, loadFun function);

	// returns size
	ENGINE_API static uvec2 load(std::string filename, std::vector<uint8>& data);

	ENGINE_API static void cleanUp();

private:
	static std::map<std::string, loadFun > loadFunctions;

};