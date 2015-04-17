#ifdef _MSC_VER
	#ifdef OpenGLRendererSource
		#define OpenGLRendererPlugin_API __declspec(dllexport)
	#else
		#define OpenGLRendererPlugin_API __declspec(dllimport)
	#endif
#endif