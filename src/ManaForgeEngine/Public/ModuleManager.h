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

	using contentModuleSpawnFun = std::function<void*(const std::string&) >;
	using updateFun = std::function<bool(float) >;
	using initFun = std::function<void()>;

	ENGINE_API ModuleManager();
	ENGINE_API ~ModuleManager();

	ENGINE_API void loadModule(const std::string& name);

	template <typename T>
	inline void registerClass(const std::string& moduleName, T* ptr = nullptr);

	ENGINE_API void addInitCallback(const initFun& function);
	ENGINE_API void addUpdateCallback(const updateFun& function);

	template <typename T>
	inline T* spawnClass(const std::string& moduleName, const std::string& className);

  private:
	// and finally the modules
	std::unordered_map<std::string, std::shared_ptr<Module>> loadedModules;

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

#include <boost/type_index.hpp>

template <typename T>
inline void ModuleManager::registerClass(const std::string& moduleName, T* ptr)
{
	std::string name = boost::typeindex::type_id<T>().pretty_name();

	std::string::iterator iterAt = name.end();
	for (auto iter = name.begin(); iter != name.end(); ++iter)
	{
		if (isspace(*iter)) {
			iterAt = iter;
			break;
		}
	}

	if (iterAt != name.end()) {
		std::string strTemp;
		std::copy(iterAt + 1, name.end(), std::back_inserter(strTemp));

		name = strTemp;
	}


	auto moduleIter = loadedModules.find(moduleName);
	if (moduleIter != loadedModules.end()) {
		moduleIter->second->addClass(name, []()
		                             {
			                             return new T();
			                         });
	}
}

template <typename T>
inline T* ModuleManager::spawnClass(const std::string& moduleName, const std::string& className)
{

	auto moduleIter = loadedModules.find(moduleName);

	if (moduleIter != loadedModules.end()) {

		return static_cast<T*>(moduleIter->second->spawnClass(className));
	}
	// if we didn't return already, it wasn't found.
	return nullptr;
}
