#pragma once
#define ENGINE_VERSION 0.1f
#define Engine_ModuleName "Engine"

#define BOOST_SERIALIZATION_DYN_LINK 1

#ifdef ENGINE
#	define ENGINE_API _declspec(dllexport)
#else
#	define ENGINE_API _declspec(dllimport)
// link to Engine.lib if we arn't compiling the engine
#	pragma message("Linking to engine...")
#	pragma comment(lib, "Engine.lib")
#endif

// ALWAYS link to stackwalker.lib
#pragma comment(lib, "StackWalker.lib")

#if defined WIN32 || defined _WIN32
#	include <Windows.h>
#	include <direct.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include <StackWalker.h>

#include <boost/preprocessor/cat.hpp>


// worldwide typedefs
typedef int8_t			int8;
typedef int16_t			int16;
typedef int32_t			int32;
typedef int64_t			int64;

typedef uint8_t			uint8;
typedef uint16_t		uint16;
typedef uint32_t		uint32;
typedef uint64_t		uint64;

typedef glm::vec2		vec2;
typedef glm::vec3		vec3;
typedef glm::vec4		vec4;

typedef glm::uvec2		uvec2;
typedef glm::uvec3		uvec3;
typedef glm::uvec4		uvec4;

typedef glm::ivec2		ivec2;
typedef glm::ivec3		ivec3;
typedef glm::ivec4		ivec4;

typedef glm::mat2		mat2;
typedef glm::mat3		mat3;
typedef glm::mat4		mat4;

typedef glm::dvec2		dvec2;
typedef glm::dvec3		dvec3;
typedef glm::dvec4		dvec4;
