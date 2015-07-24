#pragma once 
#include "Engine.h"

#include <map>
#include <vector>
#include <functional>
#include <list>
#include <typeinfo>

#include <unordered_map>


#include <boost/algorithm/string.hpp>
#include <boost/core/noncopyable.hpp>

class Runtime;
class World;
class Renderer;
class PhysicsSystem;
class AudioSystem;
class Module;


class ModuleManager : boost::noncopyable
{
public:

	friend Runtime;

	using contentModuleSpawnFun		= std::function<void*(const std::string&)>;
	using updateFun					= std::function<bool(float)>;
	using initFun					= std::function<void()>;

	ENGINE_API ModuleManager();
	ENGINE_API ~ModuleManager();

	ENGINE_API void loadModule(const path_t& filename);

	template<typename T>
	inline void registerClass(const std::string& moduleName, T* ptr = nullptr);
	
	ENGINE_API void addInitCallback(const initFun& function);
	ENGINE_API void addUpdateCallback(const updateFun& function);

	template <typename T>
	inline T* spawnClass(const std::string& name);

private:

	// and finally the modules
	std::unordered_map<path_t, std::shared_ptr<Module> > loadedModules;

	// destroy the callbacks first
	std::list<initFun>& getInitCallbacks();
	std::list<updateFun>& getUpdateCallbacks();

	std::list<initFun> initCallbacks;
	std::list<updateFun> updateCallbacks;


};

#include "Helper.h"
#include "Module.h"

#include "PhysicsSystem.h"
#include "AudioSystem.h"
#include "Renderer.h"


template<typename T>
inline void ModuleManager::registerClass(const std::string& moduleName, T* ptr)
{
	std::string nameWithClass = typeid(T).name();
	auto iterAtSpace = nameWithClass.rbegin();
	while (iterAtSpace != nameWithClass.rend() && *iterAtSpace != ' ') iterAtSpace++;

	std::string name;
	std::copy(iterAtSpace.base(), nameWithClass.end(), std::back_inserter(name));
	
	auto moduleIter = loadedModules.find(moduleName);
	if (moduleIter != loadedModules.end())
	{
		moduleIter->second->addClass(name, 
			[]()
			{
				return new T(); 
			}
		);
	}

}

template <typename T>
inline T* ModuleManager::spawnClass(const std::string& name)
{
	auto iterWhereDot = name.begin();
	while (iterWhereDot != name.end() && *iterWhereDot != '.') ++iterWhereDot;

	std::string moduleName;
	std::copy(name.begin(), iterWhereDot, std::back_inserter(moduleName));

	std::string className;
	std::copy(iterWhereDot + 1, name.end(), std::back_inserter(className));

	auto moduleIter = loadedModules.find(moduleName);

	if (moduleIter != loadedModules.end())
	{

		return static_cast<T*>(moduleIter->second->spawnClass(className));
	}
	// if we didn't return already, it wasn't found.
	return nullptr;
}

