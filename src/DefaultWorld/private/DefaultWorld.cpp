#ifndef SAVE_TYPE_XML
#define SAVE_TYPE_XML 0
#endif

#include "DefaultWorld.h"
#include "DefaultWorldLocation.h"

#include <Helper.h>
#include <PropertyManager.h>
#include <Runtime.h>
#include <Color.h>
#include <TextureLibrary.h>
#include <Renderer.h>
#include <MaterialInstance.h>
#include <Pawn.h>
#include <PlayerController.h>
#include <ModelData.h>

#include <lodepng.h>

#include <list>
#include <fstream>
#include <functional>
#include <map>
#include <vector>

#include <boost/algorithm/string.hpp>

#include <boost/serialization/deque.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>

#if SAVE_TYPE_XML
#include <boost/archive/polymorphic_xml_oarchive.hpp>
#include <boost/archive/polymorphic_xml_iarchive.hpp>
#define IS_SAVE_BINARY 0
using oarchive_t = boost::archive::polymorphic_xml_oarchive;
using iarchive_t = boost::archive::polymorphic_xml_iarchive;
#else
#include <boost/archive/polymorphic_binary_oarchive.hpp>
#include <boost/archive/polymorphic_binary_iarchive.hpp>
#define IS_SAVE_BINARY 1
using oarchive_t = boost::archive::polymorphic_binary_oarchive;
using iarchive_t = boost::archive::polymorphic_binary_iarchive;
#endif

DefaultWorld::DefaultWorld(const std::string& name)
{
	if (name != "") {
		init(name);
	}
}

DefaultWorld::~DefaultWorld()
{

	while (actors.end() != actors.begin())
	{
		delete *(--actors.end());
	}

	assert(actors.size() == 0);
}

void DefaultWorld::init(const std::string& name)
{

	folderLocation = std::string("Worlds\\") + name + '\\';
	propManager.init(folderLocation + "world.json");

	auto drawMaterial = std::shared_ptr<MaterialInstance>{Runtime::get().renderer->newMaterialInstance(
		Runtime::get().renderer->getMaterialSource("boilerplate"))};

	auto backgroundImages = std::shared_ptr<TextureLibrary>{
		Runtime::get().renderer->newTextureLibrary()}; // TODO: less hardcoded values
	backgroundImages->init(4, 256);

	// Make sure a world folder was supplied.
	if (name.empty() || name == "") {
		MFLOG(Fatal) << "No world specified";
	}

	LOAD_PROPERTY_WITH_WARNING(
		propManager, "DefaultPlayerController.Module", playerControllerModuleName, "Core");
	LOAD_PROPERTY_WITH_WARNING(propManager, "DefaultPawn.Module", pawnModuleName, "Core");

	LOAD_PROPERTY_WITH_WARNING(
		propManager, "DefaultPlayerController.Name", playerControllerClassName, "PlayerController");
	LOAD_PROPERTY_WITH_WARNING(propManager, "DefaultPawn.Name", pawnClassName, "Pawn");

	std::map<Color, std::string> imageToTextureAssoc;
	// load the image associations
	try
	{
		// We should probably just have the images we use in the same file as chunk size.
		std::ifstream stream
		{
			folderLocation + "images.txt"
#if IS_SAVE_BINARY
				,
				std::ios::binary
#endif
		};

		if (!stream.is_open()) {
			MFLOG(Fatal) << "Could not open images.txt file for world: " << worldName;
		}

		iarchive_t arch{stream}; // this might want to be not xml, maybe text or binary

		// load the map from the file
		arch >> boost::serialization::make_nvp("assoc", imageToTextureAssoc);

		backgroundImages->setFilterMode(Texture::FilterMode::MIPMAP_LINEAR);

		// load the images to the backgroundImages textureLibrary
		for (auto& elem : imageToTextureAssoc)
		{
			backgroundImages->addImage(elem.second);
		}
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
	drawMaterial->setTexture(0, backgroundImages);

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
			iarchive_t i_archive{i_stream};

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
			iarchive_t i_archive{i_stream};
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

		background = std::make_unique<ChunkActor* []>(numBackgroundChunks.x * numBackgroundChunks.y);

		auto locations = std::vector<vec2>{backgroundChunkSize * backgroundChunkSize * 4};
		auto UVs = std::vector<vec2>{backgroundChunkSize * backgroundChunkSize * 4};
		auto elems = std::vector<uvec3>{backgroundChunkSize * backgroundChunkSize * 2};

		// generate location and elem data -- all the same
		for (uint16 yTiles = 0; yTiles < backgroundChunkSize; ++yTiles)
		{
			for (uint16 xTiles = 0; xTiles < backgroundChunkSize; ++xTiles)
			{
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
		for (uint16 yChunks = 0; yChunks < numBackgroundChunks.y; ++yChunks)
		{
			for (uint16 xChunks = 0; xChunks < numBackgroundChunks.x; ++xChunks)
			{
				for (uint16 yTiles = 0; yTiles < backgroundChunkSize; ++yTiles)
				{
					for (uint16 xTiles = 0; xTiles < backgroundChunkSize; ++xTiles)
					{

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

				auto modelData = Runtime::get().renderer->newModelData();
				modelData->init(&locations[0],
					&UVs[0],
					backgroundChunkSize * backgroundChunkSize * 4,
					&elems[0],
					backgroundChunkSize * backgroundChunkSize * 2);

				background[yChunks * numBackgroundChunks.x * xChunks] = new ChunkActor(
					Transform{vec2(xChunks * backgroundChunkSize, yChunks * backgroundChunkSize)},
					drawMaterial,
					std::move(modelData));
			}
		}
	}
	// end background loading

	MFLOG(Trace) << "World Loaded!";
}

bool DefaultWorld::update(float deltaTime)
{
	tickingActors(deltaTime);
	return true;
}

std::unique_ptr<ActorLocation> DefaultWorld::addActor(Actor& toAdd)
{
	actors.push_back(&toAdd);

	return std::make_unique<DefaultWorldLocation>(actors.size() - 1, *this);

	// if we haven't returned by now, there is a problem.
	MFLOG(Fatal) << "Failed to insert into actors map";
	return nullptr;
}

void DefaultWorld::saveWorld()
{
	MFLOG(Trace) << "Saving world";
	// Create list to save
	std::deque<Actor*> toSave;
	for (auto& elem : actors)
	{
		if (elem->getSaved()) {
			toSave.push_back(elem);
		}
	} // End for

	// File location of static actors -- if we are binary, then use a binary stream
	std::ofstream o_stream
	{
		folderLocation + worldName + '\\' + worldName + ".SAVE",
#if IS_SAVE_BINARY
			std::ifstream::binary
#endif
	};

	try
	{

		// Create archive.
		oarchive_t o_archive{o_stream};
		// Save.
		o_archive << BOOST_SERIALIZATION_NVP(toSave);
	}
	catch (boost::archive::archive_exception& e)
	{
		MFLOG(Error) << "ARCHIVE ERROR SAVING ACTORS. Reason: " << e.what() << " Code: " << e.code;
	}
	catch (std::exception& e)
	{
		MFLOG(Error) << "ERROR SAVING ACTORS. Reason: " << e.what();
	}
}

std::unique_ptr<PlayerController> DefaultWorld::makePlayerController()
{
	return std::unique_ptr<PlayerController>{Runtime::get().moduleManager.spawnClass<PlayerController>(
		playerControllerModuleName, playerControllerClassName)};
}

std::unique_ptr<Pawn> DefaultWorld::makePawn()
{
	return std::unique_ptr<Pawn>{
		Runtime::get().moduleManager.spawnClass<Pawn>(pawnModuleName, pawnClassName)};
}

boost::signals2::connection DefaultWorld::registerTickingActor(const std::function<void(float)>& tickFun)
{
	return tickingActors.connect(tickFun);
}
