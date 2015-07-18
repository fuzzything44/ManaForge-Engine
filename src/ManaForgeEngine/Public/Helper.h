#pragma once
#include <Engine.h>
#include <ENGException.h>

#include <SaveData.h>

// standard library includes
#include <ios>

#include <boost/filesystem/fstream.hpp>

/// <summary> Loads file to string.</summary>
///
/// <param name="filename"> Filename of the file.</param>
///
/// <returns> The file to string.</returns>
inline std::string loadFileToStr(const path_t& filename)
{


	// stream for the file
	boost::filesystem::ifstream stream;
	stream.open(filename);

	// if the steam if bad then return
	if (!stream.is_open())
	{
		logger<Warning>() << "file doens't exist: " << filename;
		return std::string();
	}
	// define strings for each line and the final string
	std::string ret, build;

	// while there is another line, append it to ret.
	while (std::getline(stream, build))
	{
		ret += build;
		ret += "\n";
	}

	return ret;
	

}
///// <summary> Gets the string assiociated with the error code.</summary>
/////
///// <param name="error"> The error.</param>
/////
///// <returns> A the string that is the error.</returns>
//ENGINE_API std::string clGetErrorString(cl_int error);

// custom vector printing
inline std::ostream& operator<<(std::ostream& os, const vec2& vec)
{
	os << "X: " << vec.x << "  Y: " << vec.y;
	return os;
}

// custom vector printing
inline std::ostream& operator<<(std::ostream& os, const vec3& vec)
{
	os << "X: " << vec.x << "  Y: " << vec.y << "  X: " << vec.z;
	return os;
}

// custom vector printing
inline std::ostream& operator<<(std::ostream& os, const vec4& vec)
{
	os << "X: " << vec.x << "  Y: " << vec.y << "  X: " << vec.z << "  W: " << vec.w;
	return os;
}



// custom vector printing
inline std::ostream& operator<<(std::ostream& os, const ivec2& vec)
{
	os << "X: " << vec.x << "  Y: " << vec.y;
	return os;
}

// custom vector printing
inline std::ostream& operator<<(std::ostream& os, const ivec3& vec)
{
	os << "X: " << vec.x << "  Y: " << vec.y << "  X: " << vec.z;
	return os;
}

// custom vector printing
inline std::ostream& operator<<(std::ostream& os, const ivec4& vec)
{
	os << "X: " << vec.x << "  Y: " << vec.y << "  X: " << vec.z << "  W: " << vec.w;
	return os;
}


// custom vector printing
inline std::ostream& operator<<(std::ostream& os, const uvec2& vec)
{
	os << "X: " << vec.x << "  Y: " << vec.y;
	return os;
}

// custom vector printing
inline std::ostream& operator<<(std::ostream& os, const uvec3& vec)
{
	os << "X: " << vec.x << "  Y: " << vec.y << "  X: " << vec.z;
	return os;
}

// custom vector printing
inline std::ostream& operator<<(std::ostream& os, const uvec4& vec)
{
	os << "X: " << vec.x << "  Y: " << vec.y << "  X: " << vec.z << "  W: " << vec.w;
	return os;
}



inline std::ostream& operator<<(std::ostream& os, const mat3& mat)
{
	auto originalPrecison = os.precision();

	os.precision(4);
	os.setf(std::ios::fixed, std::ios::floatfield); // floatfield set to fixed

	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			os << mat[x][y] << " ";
		}
		os << std::endl;
	}

	// reset the precision
	os.precision(originalPrecison);
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const mat4& mat)
{
	auto originalPrecison = os.precision();

	os.precision(4);
	os.setf(std::ios::fixed, std::ios::floatfield); // floatfield set to fixed

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			os << mat[x][y] << " ";
		}
		os << std::endl;
	}

	// reset the precision
	os.precision(originalPrecison);
	return os;
}


// ONLY define these macros if we are debugging -- they could be slow
#ifdef _DEBUG
	

	/// <summary> Checks a pointer, and throws an exception if it is null.</summary>
	#define check(ptr) if(!ptr){  ::logger<Error>() << "Null pointer exception caught: " << #ptr; }

	
#else
	#define errChkCL(err)
	#define check(ptr)
#endif

#define LOAD_PROPERTY_WITH_WARNING(propertyManager, key, value, defaultValue)						\
	if(::boost::optional<decltype(value)> tempOptional =											\
		propertyManager.queryValue<decltype(value)>(key))											\
	{																								\
		value = *tempOptional;																		\
	}																								\
	else																							\
	{																								\
		logger<Warning>() << "Value from key " << key												\
			<< " doesn't exist. Using default value of: " << defaultValue;							\
	}																								\
	/**/

#define LOAD_PROPERTY_WITH_ERROR(propertyManager, key, value)										\
	if(::boost::optional<decltype(value)> tempOptional =											\
		propertyManager.queryValue<decltype(value)>(key))											\
	{																								\
		value = *tempOptional;																		\
	}																								\
	else																							\
	{																								\
		logger<Fatal>() << "Value from key " << key << " doesn't exist.";							\
	}																								\
	/**/
