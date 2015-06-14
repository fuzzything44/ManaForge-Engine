#pragma once 
#include "Engine.h"

#include <map>
#include <vector>
#include <functional>
#include <list>

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
	typedef std::function<World*(const std::string&)> newWorldFun;
	typedef std::function<bool(float)> updateFun;
	typedef std::function<void()> initFun;

	ENGINE_API ~ModuleManager();

	/// <summary> Default constructor.</summary>
	ENGINE_API ModuleManager();


	ENGINE_API Renderer& getRenderer();
	ENGINE_API AudioSystem& getAudioSystem();
	ENGINE_API PhysicsSystem& getPhysicsSystem();
	
	ENGINE_API void setRenderer(Renderer* newRenderer);
	ENGINE_API void setAudioSystem(AudioSystem* newAudioSystem);
	ENGINE_API void setPhysicsSystem(PhysicsSystem* newPhysicsSystem);

	/// <summary> Loads a module.</summary>
	///
	/// <param name="filename"> Filename of the module.</param>
	ENGINE_API void loadModule(const std::string& filename);

	/// <summary> .</summary>
	///
	/// <param name="createWorld()"> Function that creates a world </param>
	ENGINE_API void setCreateWorldFun(const newWorldFun& createWorld);

	/// <summary> Creates a new world using the function set in setCreateWorldFun </summary>
	/// <param name="path"> the name of the world to create </param>
	ENGINE_API World* newWorld(std::string path);

	ENGINE_API void AddContentModule(contentModuleSpawnFun fun, const std::string& moduleName);


	ENGINE_API void addInitCallback(const initFun& function);
	ENGINE_API void addUpdateCallback(const updateFun& function);

	template <typename T>
	T* spawnClass(const std::string& name);

private:

	std::list<initFun>& getInitCallbacks();
	std::list<updateFun>& getUpdateCallbacks();

	std::list<initFun> initCallbacks;
	std::list<updateFun> updateCallbacks;

	// function to createWorld
	newWorldFun newWorldFunction;

	std::map<std::string, Module> loadedModules;

	std::map<std::string, contentModuleSpawnFun> contentSpawnMethods;

	Renderer* renderer;
	AudioSystem* audioSystem;
	PhysicsSystem* physicsSystem;

};


template <typename T>
T* ModuleManager::spawnClass(const std::string& name)
{

	std::vector<std::string> brokenName;
	boost::algorithm::split(brokenName, name, boost::algorithm::is_any_of("."));

	// make sure we have at least two elements
	if (brokenName.size() < 2) return nullptr;

	auto iter = contentSpawnMethods.find(brokenName[0]);

	// if it existes call it
	if (iter != contentSpawnMethods.end())
	{
		// call the function
		return static_cast<T*>(iter->second(brokenName[1]));
	}
	else
	{
		// and if not then return nullptr
		return nullptr;
	}
}