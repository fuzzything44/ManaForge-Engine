#include <CoreManager.h>

#include "COpenGLModel.h"

using OpenGLRendererManager_t =
	Manager
	<
		boost::mpl::vector1<COpenGLModel>
		, boost::mpl::vector0<>
		, boost::mpl::vector1<CoreManager_t>
	>;



