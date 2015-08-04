#pragma once
#include "Engine.h"

#include "SharedLibrary.h"

#include <unordered_map>

class ModuleManager;

class Module : boost::noncopyable
{

  public:
	// aliases for the register plugin function type
	using registerModuleFun = void(ModuleManager&);
	using getModuleEngineVersionFun = float();

	ENGINE_API Module(const std::string& filename);

	/// <summary> Registers the module with the ModuleManager specified</summary>
	///
	/// <param name="moduleManager"> The module manager to register the module with</param>
	ENGINE_API void registerModule(ModuleManager& mm);

	ENGINE_API void addClass(const std::string& className, const std::function<void*()>& fun);
	ENGINE_API void* spawnClass(const std::string& className);


	/// <summary> Destructor.</summary>
	ENGINE_API ~Module();

  private:
	// first so it will be destructed last.
	SharedLibrary libraryHandle;

	// address to the function to register the plugin
	registerModuleFun* registerModuleFunctionAddress;
	getModuleEngineVersionFun* getModuleEngineVersionAddress;

	std::string name;
	std::unordered_map<std::string, std::function<void*()>> classes;
};