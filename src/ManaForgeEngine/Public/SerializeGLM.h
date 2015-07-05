#pragma once

#include <Engine.h>

// serialization functions for vectors
namespace boost
{
	namespace serialization
	{
		// vectors
		template<typename Archive>
		void serialize(Archive& ar, vec2& dat, const uint32 version)
		{
			ar & boost::serialization::make_nvp("x", dat.x);
			ar & boost::serialization::make_nvp("y", dat.y);
		}
		template<typename Archive>
		void serialize(Archive& ar, vec3& dat, const uint32 version)
		{
			ar & boost::serialization::make_nvp("x", dat.x);
			ar & boost::serialization::make_nvp("y", dat.y);
			ar & boost::serialization::make_nvp("z", dat.z);
		}
		template<typename Archive>
		void serialize(Archive& ar, ivec2& dat, const uint32 version)
		{
			ar & boost::serialization::make_nvp("x", dat.x);
			ar & boost::serialization::make_nvp("y", dat.y);
		}
		template<typename Archive>
		void serialize(Archive& ar, ivec3& dat, const uint32 version)
		{
			ar & boost::serialization::make_nvp("x", dat.x);
			ar & boost::serialization::make_nvp("y", dat.y);
			ar & boost::serialization::make_nvp("z", dat.z);
		}
		template<typename Archive>
		void serialize(Archive& ar, uvec2& dat, const uint32 version)
		{
			ar & boost::serialization::make_nvp("x", dat.x);
			ar & boost::serialization::make_nvp("y", dat.y);
		}
		template<typename Archive>
		void serialize(Archive& ar, uvec3& dat, const uint32 version)
		{
			ar & boost::serialization::make_nvp("x", dat.x);
			ar & boost::serialization::make_nvp("y", dat.y);
		}

		// matricies
		template<typename Archive>
		void serialize(Archive& ar, mat2& dat, const uint32 version)
		{
			ar & boost::serialization::make_nvp("0", dat[0]);
			ar & boost::serialization::make_nvp("1", dat[1]);
		}
		template<typename Archive>
		void serialize(Archive& ar, mat3& dat, const uint32 version)
		{
			ar & boost::serialization::make_nvp("0", dat[0]);
			ar & boost::serialization::make_nvp("1", dat[1]);
			ar & boost::serialization::make_nvp("2", dat[2]);
		}
		template<typename Archive>
		void serialize(Archive& ar, mat4& dat, const uint32 version)
		{
			ar & boost::serialization::make_nvp("0", dat[0]);
			ar & boost::serialization::make_nvp("1", dat[1]);
			ar & boost::serialization::make_nvp("2", dat[2]);
			ar & boost::serialization::make_nvp("3", dat[3]);
		}
	}
}