#pragma once

#include <Engine.h>

#include <unordered_map>

#include <../OpenGLRenderer/Public/OpenGLRendererManager.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <boost/archive/xml_iarchive.hpp>

struct Color
{
	uint8 red;
	uint8 green;
	uint8 blue;
	uint8 alpha;
};

// hash for Color
namespace std
{
	template<>
	struct hash<Color>
	{
		// combine it into a uint32
		size_t operator()(const Color& col) const
		{
			return std::hash<uint32>()(
				(uint32)col.red << 24
					|
				(uint32)col.green << 16
					|
				(uint32)col.blue << 8
					|
				(uint32)col.alpha
			);
		}
	};
}

template<bool isBinary = true>
inline void loadWorld(path_t path)
{
	assert(!path.empty());

	path_t savefile = L"Worlds\\" + path.wstring() + L"savefile.SAVE";
	path_t worldfile = L"Worlds\\" + path.wstring() + L"worldfile.WORLD";
	path_t assocfile = L"Worlds\\" + path.wstring() + L"images.txt";

	boost::property_tree::ptree props;
	boost::property_tree::read_json("Worlds\\" + path.string() + "\\world.json", props);
	

	auto drawMaterial = std::make_shared<OpenGLMaterialInstance>(OpenGLMaterialSource{ "boilerplate" });
//	auto backgroundImages = std::shared_ptr<TextureLibrary>{ Runtime::get().getRenderer().newTextureLibrary() };


	std::unordered_map<Color, std::string> imageToTextureAssoc;

	// load the image associations
	try
	{
		// get the stream
		auto stream = 
		[] -> std::ifstream
		{
			if(isBinary)
			{
				return std::ifstream(assocfile, std::ios::binary)
			}
			return std::ifstream(assocFile);
		}();

		if (!stream.is_open()) {
			MFLOG(Error) << "Could not open images.txt file for world: " << worldName;
		}

		boost::archive::xml_iarchive arch{ stream }; // this might want to be not xml, maybe text or binary

								   // load the map from the file
		arch >> boost::serialization::make_nvp("assoc", imageToTextureAssoc);

		// backgroundImages->init(imageToTextureAssoc.size(), 256); // TODO: less hardcoded values

		// load the images to the backgroundImages textureLibrary
		//for (auto& elem : imageToTextureAssoc) {
		//	backgroundImages->addImage(elem.second);
		//}
	}
	catch (boost::archive::archive_exception& e)
	{
		MFLOG(Error) << "ARCHIVE ERROR ENCOUNTERED WHEN LOADING IMAGE ASSOCIATIONS. Error code: " << e.code
			<< " Error message: " << e.what();
	}
	catch (std::exception& e)
	{
		MFLOG(Error) << "ERROR ENCOUNTERED WHEN LOADING IMAGE ASSOCIATIONS. Message: " << e.what();
	}

	// set the material
	//drawMaterial->textures[0] = backgroundImages;

	MFLOG(Trace) << "Loading world " << name << "...";

	/////////////////////////////////////////////////////
	// Begin static actor loading.
	{

		// List it will read things into.
		std::list<Actor*> staticActors;

		// File location of static actors -- if we are binary, then use a binary stream
		std::ifstream i_stream
		{
			folderLocation + "worldfile.WORLD",
#if IS_SAVE_BINARY
			std::ifstream::binary
#endif
		};

		// make sure that the file exists
		if (!i_stream.is_open()) {
			MFLOG(Fatal) << "WORLD FILE DOESN'T EXIST FOR WORLD " << name;
		}

		// this might fail, so put it into try catch block
		try
		{

			// Create archive.
			iarchive_t i_archive{ i_stream };

			i_archive >> BOOST_SERIALIZATION_NVP(staticActors);
		}
		catch (boost::archive::archive_exception& e)
		{
			MFLOG(Fatal) << "ARCHIVE ERROR ENCOUNTERED WHILE LOADING WORLD ACTORS! Reason: " << e.what()
				<< // do this for stack tracing
				" Error code: " << e.code;
		}
		catch (std::exception& e)
		{
			MFLOG(Fatal) << "ERROR ENCOUNTERED WHILE LOADING WORLD ACTORS! Reason: " << e.what();
		}
	}
	// End static actor loading
	////////////////////////////////////////////////////////
	// Begin dynamic actor loading
	{
		// File location of dynamic actors.
		// File location of static actors -- if we are binary, then use a binary stream
		std::ifstream i_stream
		{
			folderLocation + "savefile.SAVE",
#if IS_SAVE_BINARY
			std::ifstream::binary
#endif
		};

		if (!i_stream.is_open()) {
			MFLOG(Fatal) << "SAVE FILE DOESN'T EXIST";
		}

		// List actors will be read into.
		std::list<Actor*> dynamicActors;

		try
		{
			// Create archive.
			iarchive_t i_archive{ i_stream };
			i_archive >> BOOST_SERIALIZATION_NVP(dynamicActors);
		}
		catch (boost::archive::archive_exception& e)
		{
			MFLOG(Fatal) << "ARCHIVE ERROR WHILE LOADING SAVED ACTORS! Reason: " << e.what()
				<< " Error code: " << e.code;
		}
		catch (std::exception& e)
		{
			MFLOG(Fatal) << "ERROR ENCOUNTERED WHILE LOADING SAVED ACTORS! Reason: " << e.what();
		}
	}
	MFLOG(Trace) << "Actors Loaded!";

	///////////////////////////
	// begin background loading
	{

		using namespace std::string_literals;

		LOAD_PROPERTY_WITH_ERROR(propManager, "chunk.size", backgroundChunkSize);

		std::vector<uint8> data;
		uvec2 size;
		lodepng::decode(data, size.x, size.y, folderLocation + "background.png"s);
		numBackgroundChunks = size / backgroundChunkSize;

		background = std::make_unique<ChunkActor*[]>(numBackgroundChunks.x * numBackgroundChunks.y);

		auto locations = std::vector<vec2>{ backgroundChunkSize * backgroundChunkSize * 4 };
		auto UVs = std::vector<vec2>{ backgroundChunkSize * backgroundChunkSize * 4 };
		auto elems = std::vector<uvec3>{ backgroundChunkSize * backgroundChunkSize * 2 };

		// generate location and elem data -- all the same
		for (uint16 yTiles = 0; yTiles < backgroundChunkSize; ++yTiles) {
			for (uint16 xTiles = 0; xTiles < backgroundChunkSize; ++xTiles) {
				uint32 startIndex = yTiles * backgroundChunkSize * 4 + xTiles * 4;

				locations[startIndex] = vec2(xTiles, yTiles);
				locations[startIndex + 1] = vec2(xTiles, yTiles + 1);
				locations[startIndex + 2] = vec2(xTiles + 1, yTiles);
				locations[startIndex + 3] = vec2(xTiles + 1, yTiles + 1);

				elems[yTiles * backgroundChunkSize * 2 + xTiles * 2] =
					uvec3(startIndex, startIndex + 1, startIndex + 2);
				elems[yTiles * backgroundChunkSize * 2 + xTiles * 2 + 1] =
					uvec3(startIndex + 1, startIndex + 2, startIndex + 3);
			}
		}

		// generate UV
		for (uint16 yChunks = 0; yChunks < numBackgroundChunks.y; ++yChunks) {
			for (uint16 xChunks = 0; xChunks < numBackgroundChunks.x; ++xChunks) {
				for (uint16 yTiles = 0; yTiles < backgroundChunkSize; ++yTiles) {
					for (uint16 xTiles = 0; xTiles < backgroundChunkSize; ++xTiles) {

						uint32 startColIndex =
							((numBackgroundChunks.y - yChunks - 1) * numBackgroundChunks.x + xChunks) * 4
							* backgroundChunkSize * backgroundChunkSize
							+ ((backgroundChunkSize - yTiles - 1) * backgroundChunkSize * 4) + (4 * xTiles);

						Color col = Color(data[startColIndex],
							data[startColIndex + 1],
							data[startColIndex + 2],
							data[startColIndex + 3]);

						auto imageIter = imageToTextureAssoc.find(col);

						// if it exists
						if (imageIter != imageToTextureAssoc.end()) {
							std::string imageName = imageIter->second;

							QuadUVCoords coords;
							if (auto optCoords = backgroundImages->getUVCoords(imageName)) {
								coords = *optCoords;
							}
							else
							{
								MFLOG(Error) << "Texture coordinates not found for image: " << imageName;
							}

							uint32 startTileIndex = (yTiles * backgroundChunkSize * 4) + (xTiles * 4);

							UVs[startTileIndex] = coords.lowerLeft;
							UVs[startTileIndex + 1] = coords.upperLeft;
							UVs[startTileIndex + 2] = coords.lowerRight;
							UVs[startTileIndex + 3] = coords.upperRight;
						}
						else
						{
							MFLOG(Warning)
								<< "Warining: could not find image name in imageAssoc map in DefaultWorld";
						}
					}
				}

				auto modelData = Runtime::get().getRenderer().newModelData();
				modelData->init(&locations[0],
					&UVs[0],
					backgroundChunkSize * backgroundChunkSize * 4,
					&elems[0],
					backgroundChunkSize * backgroundChunkSize * 2);

				background[yChunks * numBackgroundChunks.x * xChunks] = new ChunkActor(
					Transform{ vec2(xChunks * backgroundChunkSize, yChunks * backgroundChunkSize) },
					drawMaterial,
					std::move(modelData));
			}
		}
	}
	// end background loading

	MFLOG(Trace) << "World Loaded!";
}
