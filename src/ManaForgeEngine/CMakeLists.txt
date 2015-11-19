include(${CMAKE_SOURCE_DIR}/CMake/build.cmake)

project(ManaForgeEngine)


set(MANAFORGE_ENGINE_PRIVATE_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/ChangeDirectory.cpp;"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/CoreManager.cpp;"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/EnginePCH.cpp;"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Logging.cpp;"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/ModuleHandler.cpp;"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/PropertyManager.cpp;"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Runtime.cpp;"
	)
	
set(MANAFORGE_ENGINE_PUBLIC_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cacher.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/call_from_tuple.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ChangeDirectory.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Color.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/CoreManager.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Engine.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/glm-ortho-2d.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Helper.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/KeyEnum.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Logging.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Manager.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/MeshActor.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModuleHandler.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/MPLHelp.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/PropertyManager.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/QuadUVCoords.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Quantity.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RefCounted.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Runtime.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/SaveData.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/SerializeGLM.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/TextBoxWidget.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/TimerHandle.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/TimerManager.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Transform.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Manager/Callbacks.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Manager/Entity.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Manager/FindAllManagers.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Manager/GetRefToManager.h;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Manager/MiscMetafunctions.h;"
	)


	
buildmodule(ManaForgeEngine "${MANAFORGE_ENGINE_PRIVATE_FILES}${MANAFORGE_ENGINE_PUBLIC_FILES}")  

