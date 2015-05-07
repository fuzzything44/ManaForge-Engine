#pragma once
#include <Engine.h>
#include <ENGException.h>

#include <boost/serialization/nvp.hpp>

// OpenCL include
#include <CL/cl.hpp>

// standard library includes
#include <ios>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

/// <summary> Loads file to string.</summary>
///
/// <param name="filename"> Filename of the file.</param>
///
/// <returns> The file to string.</returns>
ENGINE_API std::string loadFileToStr(const char* filename);

/// <summary> Loads the shaders.</summary>
///
/// <param name="vertexFile">   The vertex file.</param>
/// <param name="fragmentFile"> The fragment file.</param>
///
/// <returns> The shaders.</returns>
ENGINE_API uint32 LoadShaders(const char* vertexFile, const char* fragmentFile);

/// <summary> Loads the DDS.</summary>
///
/// <param name="imagepath"> The imagepath.</param>
///
/// <returns> The DDS.</returns>
ENGINE_API uint32 loadDDS(const char * imagepath);

/// <summary> Gets the string assiociated with the error code.</summary>
///
/// <param name="error"> The error.</param>
///
/// <returns> A the string that is the error.</returns>
ENGINE_API std::string clGetErrorString(cl_int error);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const vec2& vec);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const vec3& vec);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const vec4& vec);


/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const ivec2& vec);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const ivec3& vec);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const ivec4& vec);


/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const uvec2& vec);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const uvec3& vec);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const uvec4& vec);

/// <summary> custom matrix printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const mat3& mat);

/// <summary> custom matrix printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const mat4& mat);

// serialization functions for vectors
namespace boost
{
	namespace serialization
	{
		template<typename Archive>
		void serialize(Archive& ar, vec2& dat, const uint32 version)
		{
			ar & BOOST_SERIALIZATION_NVP(dat.x);
			ar & BOOST_SERIALIZATION_NVP(dat.y);
		}
		template<typename Archive>
		void serialize(Archive& ar, vec3& dat, const uint32 version)
		{
			ar & BOOST_SERIALIZATION_NVP(dat.x);
			ar & BOOST_SERIALIZATION_NVP(dat.y);
			ar & BOOST_SERIALIZATION_NVP(dat.z);
		}
		template<typename Archive>
		void serialize(Archive& ar, ivec2& dat, const uint32 version)
		{
			ar & BOOST_SERIALIZATION_NVP(dat.x);
			ar & BOOST_SERIALIZATION_NVP(dat.y);
		}
		template<typename Archive>
		void serialize(Archive& ar, ivec3& dat, const uint32 version)
		{
			ar & BOOST_SERIALIZATION_NVP(dat.x);
			ar & BOOST_SERIALIZATION_NVP(dat.y);
			ar & BOOST_SERIALIZATION_NVP(dat.z);
		}
		template<typename Archive>
		void serialize(Archive& ar, uvec2& dat, const uint32 version)
		{
			ar & BOOST_SERIALIZATION_NVP(dat.x);
			ar & BOOST_SERIALIZATION_NVP(dat.y);
		}
		template<typename Archive>
		void serialize(Archive& ar, uvec3& dat, const uint32 version)
		{
			ar & BOOST_SERIALIZATION_NVP(dat.x);
			ar & BOOST_SERIALIZATION_NVP(dat.y);
			ar & BOOST_SERIALIZATION_NVP(dat.z);
		}
	}
}

// ONLY define these macros if we are debugging -- they could be slow
#ifdef _DEBUG
	

	/// <summary> A macro that defines error check cl.</summary>
	///
	/// <param name="err"> The error.</param>
	#define errChkCL(err) if(err != CL_SUCCESS){throw ENGException("OpenCL error." + clGetErrorString(err), Stack::GetTraceString());}

	/// <summary> Checks a pointer, and throws an exception if it is null.</summary>
	#define check(ptr) if(!ptr){  throw ENGException(std::string("Null pointer exception caught: ") + std::string(#ptr)); }

	
#else
	#define errChkCL(err)
	#define check(ptr)
#endif
