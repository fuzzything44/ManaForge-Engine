#pragma once
// worldwide typedefs

#define ENGINE_VERSION 1.3f

#ifdef ENGINE
	#define ENGINE_API _declspec(dllexport)
#else
	#define ENGINE_API _declspec(dllimport)
#endif


#if defined WIN32 || defined _WIN32
	#include <Windows.h>
	#include <direct.h>
#endif

#include <GLTools/glew.h>
#include <GLTools/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <Poppy/PoppyDebugTools.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::uvec2 uvec2;
typedef glm::uvec3 uvec3;
typedef glm::uvec4 uvec4;
typedef glm::ivec2 ivec2;
typedef glm::ivec3 ivec3;
typedef glm::ivec4 ivec4;
typedef glm::mat2 mat2;
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::dvec2 dvec2;
typedef glm::dvec3 dvec3;
typedef glm::dvec4 dvec4;