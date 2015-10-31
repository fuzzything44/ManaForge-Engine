#include "OpenGLRendererConfig.h"

#include <CoreManager.h>

#include "COpenGLModel.h"
#include "COpenGLWindow.h"

using OpenGLRendererManager_t =
	Manager
	<
		boost::mpl::vector2<COpenGLModel, COpenGLWindow>
		, boost::mpl::vector0<>
		, boost::mpl::vector1<CoreManager_t>
	>;


template<>
struct ManagerData<OpenGLRendererManager_t>
{
	ManagerData() = default;

	EntityHandle<OpenGLRendererManager_t> currentWindow;
};

template<>
void beginPlayManager<>(OpenGLRendererManager_t& manager)
{
	auto window = manager.createEntity();
	manager.addComponent<COpenGLWindow>(window); // default construct a Window
}


extern std::shared_ptr<OpenGLRendererManager_t> openGLRendererManager;