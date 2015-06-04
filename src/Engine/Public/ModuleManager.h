#pragma once 

#include "Engine.h"
#include "Module.h"
#include "Renderer.h"
#include "World.h"

#include <map>
#include <vector>
#include <functional>
#include <list>

#include <boost/algorithm/string.hpp>

#include <boost/core/noncopyable.hpp>

class Runtime;
class World;
class Renderer;
class AudioSystem;

class ModuleManager
	: boost::noncopyable
{
public:

	friend Runtime;

	typedef std::function<Actor*(const std::string&, const Transform&)> contentModuleSpawnFun;
	typedef std::function<World*(const std::string&)> newWorldFun;

	ENGINE_API ~ModuleManager();

	/// <summary> Default constructor.</summary>
	ENGINE_API ModuleManager(); 

	/// <summary> Gets the renderer./</summary>
	///
	/// <returns> if it fails, throws an exception, else the renderer.</returns>
	ENGINE_API Renderer& getRenderer();

	/// <summary> Gets the audio system./</summary>
	///
	/// <returns> if it fails, throws an exception, else the audio system.</returns>
	ENGINE_API AudioSystem& getAudioSystem();
		
	/// <summary> Adds a renderer.</summary>
	///
	/// <param name="newRenderer"> If non-null, the new renderer.</param>
	ENGINE_API void setRenderer(Renderer* newRenderer);


	/// <summary> Adds a renderer.</summary>
	///
	/// <param name="newRenderer"> If non-null, the new renderer.</param>
	ENGINE_API void setAudioSystem(AudioSystem* newRenderer);


	/// <summary> Loads a module.</summary>
	///
	/// <param name="filename"> Filename of the module.</param>
	ENGINE_API 	void loadModule(const std::string& filename);

	/// <summary> .</summary>
	///
	/// <param name="createWorld()"> Function that creates a world </param>
	ENGINE_API void setCreateWorldFun(const newWorldFun& createWorld);

	/// <summary> Creates a new world using the function set in setCreateWorldFun </summary>
	/// <param name="path"> the name of the world to create </param>
	ENGINE_API World* newWorld(std::string path);

	ENGINE_API void AddContentModule(contentModuleSpawnFun fun, const std::string& moduleName);

	ENGINE_API Actor* spawnActor(const std::string& name, const Transform& trans = Transform{});

	ENGINE_API void addInitCallback(const std::function<void()>& function);
	ENGINE_API void addUpdateCallback(const std::function<bool()>& function);

private:

	std::list<std::function<void()> >& getInitCallbacks();
	std::list<std::function<bool()> >& getUpdateCallbacks();

	std::list<std::function<void()> > initCallbacks;
	std::list<std::function<bool()> > updateCallbacks;

	// function to createWorld
	newWorldFun newWorldFunction;

	std::map<std::string, Module> loadedModules;

	std::map<std::string, contentModuleSpawnFun> contentSpawnMethods;

	Renderer* renderer;
	AudioSystem* audioSystem;
};
