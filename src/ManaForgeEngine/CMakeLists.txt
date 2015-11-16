cmake_minimum_required(3.0)

set(MANAFORGE_ENGINE_PRIVATE_FILES
	" \
	src/ChangeDirectory.cpp \
	src/CoreManager.cpp \
	src/ENGException.cpp \
	src/EnginePCH.cpp \
	src/Logging.cpp \
	src/ModuleHandler.cpp \
	src/PropertyManager.cpp \
	src/Runtime.cpp \
	"
	)
	
set(MANAFORGE_ENGINE_PUBLIC_FILES
	" \
	include/Cacher.h \
	include/call_from_tuple.h \
	include/ChangeDirectory.h \
	include/Color.h \
	include/CoreManager.h \
	include/ENGException.h \
	include/Engine.h \
	include/glm-ortho-2d.h \
	include/Helper.h \
	include/KeyEnum.h \
	include/Logging.h \
	include/Manager.h \
	include/MeshActor.h \
	include/ModuleHandler.h \
	include/MPLHelp.h \
	include/PropertyManager.h \
	include/QuadUVCoords.h \
	include/Quantity.h \
	include/RefCounted.h \
	include/Runtime.h \
	include/SaveData.h \
	include/SerializeGLM.h \
	include/TestBoxWidget.h \
	include/TimerHandle.h \
	include/TimerManager.h \
	include/Transform.h \
	include/Manager/Callbacks.h \
	include/Manager/Entity.h \
	include/Manager/FindAllManagers.h \
	include/Manager/GerRefToManager.h \
	include/Manager/MiscMetafunctions.h \
	"
	)

add_executable(ManaForgeEngine ${MANAFORGE_ENGINE_PRIVATE_FILES} ${MANAFORGE_ENGINE_PUBLIC_FILES})  
