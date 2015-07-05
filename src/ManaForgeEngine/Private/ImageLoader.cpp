#include "EnginePCH.h"

#include "ImageLoader.h"

std::map<std::string, std::function<uvec2(std::string, std::vector<uint8>&)> > 
	ImageLoader::loadFunctions = std::map<std::string, loadFun >();

uvec2 ImageLoader::load(std::string filename, std::vector<uint8>& data)
{
	std::string extension;

	for (uint8 i = 1; filename[filename.size() - i] != '.' && i <= filename.size(); i++)
	{
		extension.push_back(filename[filename.size() - i]);

	}

	std::reverse(extension.begin(), extension.end());

	auto foundIter = loadFunctions.find(extension);

	if (foundIter == loadFunctions.end())
	{
		logger<Warning>() << "no loader for extension, returning nullvector";

		data.clear();

		return uvec2(0);
	}

	auto fun = foundIter->second;

	check(fun);

	return fun(filename, data);
}

void ImageLoader::addLoader(std::string extension, loadFun function)
{
	if (loadFunctions.find(extension) != loadFunctions.end())
	{
		logger<Info>() << "Dulplicate loader for " << extension << ". using newest loader.";

	}

	loadFunctions[extension] = function;


}

void ImageLoader::cleanUp()
{
	for (auto& elem : loadFunctions)
	{
		elem.second.~function();
	}
}