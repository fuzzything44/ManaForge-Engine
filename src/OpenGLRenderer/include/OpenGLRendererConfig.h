#pragma once


#ifdef OpenGLRenderer_Source
#	define OpenGLRenderer_API DLLEXPORT
#else
#	define OpenGLRenderer_API DLLIMPORT
#endif
