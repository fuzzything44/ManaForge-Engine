#pragma comment(lib, "freetype.lib")

#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <bitset>

#include <cstdint>

#include "lodepng.h"

#include <boost/lexical_cast.hpp>
#include <boost/dynamic_bitset.hpp>
#include <map>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/map.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <fstream>

FT_Library ft_lib;
FT_Face face;

size_t sourceRes = 1000;
size_t targetRes = 1000;

#include <glm/glm.hpp>
#include <SerializeGLM.h>

template <typename Archive> void serialize(Archive& ar, vec2& dat, const uint32_t version)
{
	ar& boost::serialization::make_nvp("x", dat.x);
	ar& boost::serialization::make_nvp("y", dat.y);
}

struct bitmapData
{
	vec2 uvBegin;
	vec2 uvEnd;
	float advance;
	vec2 offset;
	vec2 size;

	template <typename Archive>
	void serialize(Archive& ar, unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(uvBegin);
		ar & BOOST_SERIALIZATION_NVP(uvEnd);
		ar & BOOST_SERIALIZATION_NVP(advance);
		ar & BOOST_SERIALIZATION_NVP(offset);
		ar & BOOST_SERIALIZATION_NVP(size);
	}
};


bool glyphBit(const FT_GlyphSlot &glyph, const int x, const int y)
{
	int pitch = abs(glyph->bitmap.pitch);
	unsigned char *row = &glyph->bitmap.buffer[pitch * y];
	char cValue = row[x >> 3];

	return (cValue & (128 >> (x & 7))) != 0;
}

void distanceField(std::vector<unsigned char>& out,
	const boost::dynamic_bitset<unsigned char>& in, size_t width, int searchDistance,
	size_t outX, size_t outY)
{

	size_t rows = in.size() / width;

	out.resize(outX * outY, 0);

	int amtPerPercent = outY / 100;

	for (int y = 0; y < (int)outY; ++y)
	{
		if (float(y) / float(amtPerPercent) == floor(float(y) / float(amtPerPercent)))
		{
			std::cout << y / amtPerPercent << "%" << std::endl;
		}
		for (int x = 0; x < (int)outX; ++x)
		{
			float f_locX = (float)x / (float)outX;
			float f_locY = (float)y / (float)outY;

			bool isInChar = in[size_t((f_locY * width) * width + (f_locX * rows))];

			float distance = (float)searchDistance * sqrtf(2.f);

			// search for shortest distance
			for (int searchY = -searchDistance; searchY < searchDistance; ++searchY)
			{
				for (int searchX = -searchDistance; searchX < searchDistance; ++searchX)
				{
					if (f_locX * width + searchX >= 0 && f_locX * width + searchX < width &&
						f_locY * rows + searchY >= 0 && f_locY * rows + searchY < rows)
					{

						bool isInCharTest = in[size_t((f_locY * width + searchY) * width + (f_locX * rows + searchX))];

						if (isInChar != isInCharTest)
						{
							float distanceTest = sqrtf(
								abs(float(searchX)) * abs(float(searchX)) +
								abs(float(searchY)) * abs(float(searchY))
								);

							distance = min(distance, distanceTest);
						}
					}
				}
			}

			if (isInChar)
			{
				float interp = distance / (searchDistance * sqrtf(2.f));

				interp /= 2.f;
				interp += .5f;
				interp *= 255;



				unsigned char outChar = static_cast<unsigned char>(interp);

				out[y * outX + x] = outChar;
			}
			else
			{
				float interp = distance / (searchDistance * sqrtf(2.f));

				interp /= 2.f;
				interp += .5f;
				interp = 1.f - interp;
				interp *= 255;

				unsigned char outChar = static_cast<unsigned char>(interp);

				out[y * outX + x] = outChar;
			}
		}
	}

}

void subimage_bitset(boost::dynamic_bitset<unsigned char>& in, int inWidth, int startX, int startY, FT_GlyphSlot glyph)
{
	for (int y = 0; y < glyph->bitmap.rows; ++y)
	{
		for (int x = 0; x < glyph->bitmap.width; ++x)
		{
			in[(y + startY) * inWidth + x + startX] = glyphBit(glyph, x, y);
		}
	}
}

int main(int argc, char** argv)
{

	if (argc != 2) return -1;

	if (auto error = FT_Init_FreeType(&ft_lib))
	{
		std::cout << "failed to init FT. err: " << error;
		__debugbreak();

	}



	if (auto error = FT_New_Face(ft_lib, argv[1], 0, &face))
	{
		std::cout << "failed to load " << argv[1] << ". error: " << error;

	}


	if (auto error = FT_Set_Pixel_Sizes(face, sourceRes, 0))
	{
		std::cout << "failed to set character size. err: " << error;
	}

	FT_GlyphSlot glyph = face->glyph;


	boost::dynamic_bitset<unsigned char> bitset;
	bitset.resize(sourceRes * 10 * sourceRes * 10);

	std::map<char, bitmapData> drawData;

	for (int y = 0; y < 10; ++y)
	{
		for (int x = 0; x < 10; ++x)
		{
			char c = y * 10 + x + char(32);

			if (auto error = FT_Load_Glyph(face, FT_Get_Char_Index(face, c), FT_LOAD_MONOCHROME)) { std::cout << error; __debugbreak(); }

			if (auto error = FT_Render_Glyph(glyph, FT_RENDER_MODE_MONO)) { std::cout << error; __debugbreak(); }


			bitmapData dataChar;
			dataChar.advance = (glyph->advance.x / 64.f) / float(10 * sourceRes);
			dataChar.offset.x = float(glyph->bitmap_left) / float(10 * sourceRes);
			dataChar.offset.y = float(glyph->bitmap_top) / float(10 * sourceRes);
			dataChar.size.x = glyph->bitmap.width / float(10 * sourceRes);
			dataChar.size.y = glyph->bitmap.rows / float(10 * sourceRes);
			dataChar.uvBegin.x = float(x * sourceRes + 5) / float(10 * sourceRes);
			dataChar.uvBegin.y = float(y * sourceRes + 5) / float(10 * sourceRes);
			dataChar.uvEnd.x = dataChar.uvBegin.x + dataChar.size.x;
			dataChar.uvEnd.y = dataChar.uvBegin.y + dataChar.size.y;

			drawData[c] = dataChar;

			subimage_bitset(bitset, sourceRes * 10, x * sourceRes + 5, y * sourceRes + 5, glyph);

		}
	}
	std::ofstream o_str{ "drawData.txt" };
	boost::archive::xml_oarchive o_arch{ o_str };

	o_arch << BOOST_SERIALIZATION_NVP(drawData);

	std::vector<unsigned char> data;

	distanceField(data, bitset, sourceRes * 10, 20, targetRes, targetRes);

	//for (int i = 0; i < bitset.size(); ++i)
	//{
	//	data.push_back(bitset[i] * 255);
	//}

	lodepng::encode("font.png", data, targetRes, targetRes, LCT_GREY, 8);



}

