#pragma once
#define ENGINE_VERSION 0.1f

#ifdef ManaForgeEngine_Source
#	define ENGINE_API _declspec(dllexport)
#else
#	define ENGINE_API _declspec(dllimport)
#	pragma comment(lib, "ManaForgeEngine.lib")
#endif

#ifdef USE_VLD
#	pragma message("VLD is enabled")
#	include <vld.h>
#endif

// ALWAYS link to stackwalker.lib
#pragma comment(lib, "StackWalker.lib")

#if defined WIN32 || defined _WIN32
#	define WIN32_LEAN_AND_MEAN 
#	include <Windows.h>
#	include <direct.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include <StackWalker.h>

#include "Logging.h"
#include "ENGException.h"

#include <boost/filesystem/path.hpp>

namespace std{

	template<>
	struct std::hash<boost::filesystem::path>
	{
		size_t operator()(const boost::filesystem::path& p)
		{
			size_t current = 0;
			for (auto& elem : p.generic_wstring())
			{
				current += (size_t)elem;
			}

			return current;
		}
	};
}

// worldwide aliases
using path_t = boost::filesystem::path;

using int8		= int8_t;
using int16		= int16_t;
using int32		= int32_t;
using int64		= int64_t;
 
using uint8		= uint8_t;
using uint16	= uint16_t;
using uint32	= uint32_t;
using uint64	= uint64_t;
 


using vec2		= glm::vec2;
using vec3		= glm::vec3;
using vec4		= glm::vec4;
 
using uvec2		= glm::uvec2;
using uvec3		= glm::uvec3;
using uvec4		= glm::uvec4;
 
using ivec2		= glm::ivec2;
using ivec3		= glm::ivec3;
using ivec4		= glm::ivec4;
 
using mat2		= glm::mat2;
using mat3		= glm::mat3;
using mat4		= glm::mat4;
 
using dvec2		= glm::dvec2;
using dvec3		= glm::dvec3;
using dvec4		= glm::dvec4;
