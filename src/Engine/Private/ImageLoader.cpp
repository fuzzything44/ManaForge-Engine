#include "ImageLoader.h"

std::map<std::string, std::function<uvec2(std::string, std::vector<uint8>) >* > ImageLoader::loadFunctions = 
		std::map<std::string, std::function<uvec2(std::string, std::vector<uint8>) >* >();

uvec2 ImageLoader::load(std::string filename, std::vector<uint8>& data)
{
	std::string extension;

	for (uint8 i = 1; filename[filename.size() - i] != '.' && i <= filename.size(); i++)
	{
		extension.push_back(filename[filename.size() - i]);

	}

	// flip
	for (uint8 elem = 0; elem < floor(static_cast<float>(extension.size()) / 2.f); elem++)
	{
		std::swap(extension[elem], extension[extension.size() - elem]);
	}

	if (loadFunctions.find(extension) == loadFunctions.end())
	{
		FATAL_ERR("no loader for extension");
	}

	auto fun = *loadFunctions[extension];

	check(fun);

	return fun(filename, data);
}

void ImageLoader::addLoader(std::string extension, std::function<uvec2(std::string, std::vector<uint8>)> function)
{
	if (loadFunctions.find(extension) != loadFunctions.end())
	{
		ENG_LOGLN("Dulplicate loader for " << extension << ". using newest loader.");

	}
	loadFunctions[extension] = new std::function<uvec2(std::string, std::vector<uint8>) > (function);


}