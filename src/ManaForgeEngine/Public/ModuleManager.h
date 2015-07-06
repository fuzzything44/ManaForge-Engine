#pragma once 
#include "Engine.h"

#include <map>
#include <vector>
#include <functional>
#include <list>
#include <typeinfo>


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

	typedef std::function<void*(const std::string&)> contentModuleSpawnFun;
	typedef std::function<bool(float)> updateFun;
	typedef std::function<void()> initFun;

	ENGINE_API ~ModuleManager();

	/// <summary> Default constructor.</summary>
	ENGINE_API ModuleManager();


	inline Renderer& getRenderer();
	inline AudioSystem& getAudioSystem();
	inline PhysicsSystem& getPhysicsSystem();
	
	inline void setRenderer(std::unique_ptr<Renderer> newRenderer);
	inline void setAudioSystem(std::unique_ptr<AudioSystem> newAudioSystem);
	inline void setPhysicsSystem(std::unique_ptr<PhysicsSystem> newPhysicsSystem);

	/// <summary> Loads a module.</summary>
	///
	/// <param name="filename"> Filename of the module.</param>
	ENGINE_API void loadModule(const std::string& filename);

	template<typename T>
	inline void registerClass(const std::string& moduleName, T* ptr = nullptr);
	
	ENGINE_API void addInitCallback(const initFun& function);
	ENGINE_API void addUpdateCallback(const updateFun& function);

	template <typename T>
	inline T* spawnClass(const std::string& name);

private:

	std::list<initFun>& getInitCallbacks();
	std::list<updateFun>& getUpdateCallbacks();

	std::list<initFun> initCallbacks;
	std::list<updateFun> updateCallbacks;

	std::map<std::string, std::shared_ptr<Module> > loadedModules;

	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<AudioSystem> audioSystem;
	std::unique_ptr<PhysicsSystem> physicsSystem;

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


Renderer& ModuleManager::getRenderer()
{
	check(renderer); return *renderer;
}

AudioSystem& ModuleManager::getAudioSystem()
{
	check(audioSystem); return *audioSystem;
}

PhysicsSystem& ModuleManager::getPhysicsSystem()
{
	check(physicsSystem); return *physicsSystem;
}

void ModuleManager::setRenderer(std::unique_ptr<Renderer> newRenderer)
{
	check(newRenderer);

	renderer = std::move(newRenderer);

}

void ModuleManager::setAudioSystem(std::unique_ptr<AudioSystem> newAudioSystem)
{
	check(newAudioSystem);

	audioSystem = std::move(newAudioSystem);
}

void ModuleManager::setPhysicsSystem(std::unique_ptr<PhysicsSystem> newPhysicsSystem)
{
	check(newPhysicsSystem);

	physicsSystem = std::move(newPhysicsSystem);
}
