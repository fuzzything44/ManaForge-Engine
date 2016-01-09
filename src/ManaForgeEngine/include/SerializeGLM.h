#pragma once

#include <Engine.h>

#include <boost/serialization/nvp.hpp>

#include <iostream>

// iostream serialization
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

	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
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

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			os << mat[x][y] << " ";
		}
		os << std::endl;
	}

	// reset the precision
	os.precision(originalPrecison);
	return os;
}

// serialization functions for vectors
namespace boost
{
namespace serialization
{
// vectors
template <typename Archive>
void serialize(Archive& ar, vec2& dat, const uint32 version)
{
	ar& boost::serialization::make_nvp("x", dat.x);
	ar& boost::serialization::make_nvp("y", dat.y);
}
template <typename Archive>
void serialize(Archive& ar, vec3& dat, const uint32 version)
{
	ar& boost::serialization::make_nvp("x", dat.x);
	ar& boost::serialization::make_nvp("y", dat.y);
	ar& boost::serialization::make_nvp("z", dat.z);
}
template <typename Archive>
void serialize(Archive& ar, ivec2& dat, const uint32 version)
{
	ar& boost::serialization::make_nvp("x", dat.x);
	ar& boost::serialization::make_nvp("y", dat.y);
}
template <typename Archive>
void serialize(Archive& ar, ivec3& dat, const uint32 version)
{
	ar& boost::serialization::make_nvp("x", dat.x);
	ar& boost::serialization::make_nvp("y", dat.y);
	ar& boost::serialization::make_nvp("z", dat.z);
}
template <typename Archive>
void serialize(Archive& ar, uvec2& dat, const uint32 version)
{
	ar& boost::serialization::make_nvp("x", dat.x);
	ar& boost::serialization::make_nvp("y", dat.y);
}
template <typename Archive>
void serialize(Archive& ar, uvec3& dat, const uint32 version)
{
	ar& boost::serialization::make_nvp("x", dat.x);
	ar& boost::serialization::make_nvp("y", dat.y);
}

// matricies
template <typename Archive>
void serialize(Archive& ar, mat2& dat, const uint32 version)
{
	ar& boost::serialization::make_nvp("0", dat[0]);
	ar& boost::serialization::make_nvp("1", dat[1]);
}
template <typename Archive>
void serialize(Archive& ar, mat3& dat, const uint32 version)
{
	ar& boost::serialization::make_nvp("0", dat[0]);
	ar& boost::serialization::make_nvp("1", dat[1]);
	ar& boost::serialization::make_nvp("2", dat[2]);
}
template <typename Archive>
void serialize(Archive& ar, mat4& dat, const uint32 version)
{
	ar& boost::serialization::make_nvp("0", dat[0]);
	ar& boost::serialization::make_nvp("1", dat[1]);
	ar& boost::serialization::make_nvp("2", dat[2]);
	ar& boost::serialization::make_nvp("3", dat[3]);
}
}
}
