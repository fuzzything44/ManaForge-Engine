#pragma once
#define ENGINE_VERSION 0.1f

#include "Config.h"

// setup dllexport and import for cross-platform
#if defined __GNUC__
#define DLLEXPORT __attribute__ ((visibility ("default")))
#define DLLIMPORT __attribute__ ((visibility ("default")))
#elif defined _MSC_VER 
#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)
#endif

#ifdef ManaForgeEngine_EXPORTS
#define MF_API DLLEXPORT
#else
#define MF_API DLLIMPORT
#endif


#pragma warning(disable:4503)


#include <glm/glm.hpp>
#include <glm/gtx/type_aligned.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include "Logging.h"

#include <boost/filesystem/path.hpp>

// hashing for path_t
namespace std
{

template <>
struct hash<boost::filesystem::path>
{

	size_t operator()(const boost::filesystem::path& p) const
	{
		return std::hash<std::wstring>()(p.wstring());
	}
};
}

// worldwide aliases
using path_t = boost::filesystem::path;

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;

using uvec2 = glm::uvec2;
using uvec3 = glm::uvec3;
using uvec4 = glm::uvec4;

using ivec2 = glm::ivec2;
using ivec3 = glm::ivec3;
using ivec4 = glm::ivec4;

using mat2 = glm::mat2;
using mat3 = glm::mat3;
using mat4 = glm::mat4;

using dvec2 = glm::dvec2;
using dvec3 = glm::dvec3;
using dvec4 = glm::dvec4; 
