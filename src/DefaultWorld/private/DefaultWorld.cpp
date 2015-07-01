#ifndef SAVE_TYPE_XML
#	define SAVE_TYPE_XML 0
#endif

#include "DefaultWorld.h"
#include "DefaultWorldLocation.h"

#include <Helper.h>
#include <ImageLoader.h>
#include <PropertyManager.h>
#include <Runtime.h>
#include <Color.h>
#include <TextureLibrary.h>
#include <Renderer.h>
#include <SaveData.h>

#include <list>
#include <fstream>
#include <functional>
#include <map>
#include <vector>

#include <boost/algorithm/string.hpp>



#if SAVE_TYPE_XML
#	include <boost/archive/polymorphic_xml_oarchive.hpp>
#	include <boost/archive/polymorphic_xml_iarchive.hpp>
#else
#	include <boost/archive/polymorphic_binary_oarchive.hpp>
#	include <boost/archive/polymorphic_binary_iarchive.hpp>
#endif


// Defines for in and out archives. May want to change to defines and include the istreams.
#if SAVE_TYPE_XML
	typedef boost::archive::polymorphic_xml_oarchive oarchive_t;
	typedef boost::archive::polymorphic_xml_iarchive iarchive_t;
#	define IS_SAVE_BINARY 0
#else
	typedef boost::archive::polymorphic_binary_oarchive oarchive_t;
	typedef boost::archive::polymorphic_binary_iarchive iarchive_t;
#	define IS_SAVE_BINARY 1
#endif

using namespace std;


DefaultWorld::DefaultWorld()
{
}


DefaultWorld::~DefaultWorld()
{

	if (backgroundImages)
		delete backgroundImages;
	
	if (drawMaterial)
		delete drawMaterial;

	for (auto& elem : actors)
	{
		if (elem.second)
			delete elem.second;
	}
}


void DefaultWorld::init(const std::string& name)
{

	folderLocation = std::string("Worlds\\") + name + '\\';
	propManager.init(folderLocation + "world.json");
	nextIndex = 0;
	backgroundImages = Runtime::get().moduleManager.getRenderer().newTextureLibrary(4, 256); // TODO: less hardcoded values
	drawMaterial = Runtime::get().moduleManager.getRenderer().newMaterial("boilerplate");


	// Make sure a world folder was supplied.
	if (name == "") {
		FATAL_ERR("No world specified");
	}
	ENG_LOGLN(Trace) << "Setting up console commands...";
	// Code for console commands goes here.
	// We probably want addActor, save, loadWorld.
	// Should this actually go here..?? 

	ENG_LOGLN(Trace) << "Loading images...";


	LOAD_PROPERTY_WITH_WARNING(propManager, "DefaultPlayerController", playerControllerClassName, "Core.PlayerController");
	LOAD_PROPERTY_WITH_WARNING(propManager, "DefaultPawn", pawnClassName, "Core.Pawn");


	// We should probably just have the images we use in the same file as chunk size.
	std::ifstream stream{ folderLocation + "images.txt", std::ifstream::app };

	if (!stream.is_open())
	{
		FATAL_ERR("Could not open images.txt file for world: " + worldName);
	}

	boost::archive::xml_iarchive arch{ stream }; // this might want to be not xml, maybe text or binary



	// load the map from the file
	arch >> BOOST_SERIALIZATION_NVP(imageToTextureAssoc);

	backgroundImages->setFilterMode(Texture::FilterMode::MIPMAP_LINEAR);

	// load the images to the backgroundImages textureLibrary
	for (auto& elem : imageToTextureAssoc)
	{
		backgroundImages->addImage(elem.second);
	}

	// set the material
	drawMaterial->setTexture(0, reinterpret_cast<Texture*>(backgroundImages));

	ENG_LOGLN(Trace) << "Images loaded!";

	// virtual functuion -- call THIS version of it -- we need this because it is inside a constructor
	DefaultWorld::loadWorld("main");
}


void DefaultWorld::loadWorld(std::string name)
{
	worldName = name;
	ENG_LOGLN(Trace) << "Loading world " << name << "...";

	/////////////////////////////////////////////////////
	// Begin static actor loading.
	{


		// List it will read things into.
		std::list<Actor*> staticActors;

		// File location of static actors -- if we are binary, then use a binary stream
		std::ifstream i_stream{ folderLocation + name + '\\' + name + ".WORLD",
#	if IS_SAVE_BINARY
		std::ifstream::binary
#	endif
		};

		

		if (!i_stream.is_open())
		{
			FATAL_ERR("WORLD FILE DOESN'T EXIST");
		}

		// this might fail, so put it into try catch block
		try{

			// Create archive.
			iarchive_t i_archive{ i_stream };


			i_archive >> BOOST_SERIALIZATION_NVP(staticActors);

		} 
		catch (boost::archive::archive_exception& e)
		{
			FATAL_ERR(std::string("ARCHIVE ERROR ENCOUNTERED WHILE LOADING WORLD ACTORS! Reason: ") + e.what() + // do this for stack tracing
				" Error code: " + boost::lexical_cast<std::string>(e.code));
		}
		catch (std::exception& e)
		{
			FATAL_ERR(std::string("ERROR ENCOUNTERED WHILE LOADING WORLD ACTORS! Reason: ") + e.what());
		}

		// Move actors to the map. GUIDs are generated by default Actor constructor
		for (auto& elem : staticActors) 
		{
			actors[static_cast<DefaultWorldLocation*>(elem->GUID)->location] = elem;
		}
	}
	// End static actor loading
	////////////////////////////////////////////////////////
	// Begin dynamic actor loading
	{
		// File location of dynamic actors.
		// File location of static actors -- if we are binary, then use a binary stream
		std::ifstream i_stream{ folderLocation + name + '\\' + name + ".SAVE",
#	if IS_SAVE_BINARY
			std::ifstream::binary
#	endif
	};

		if (!i_stream.is_open())
		{
			FATAL_ERR("SAVE FILE DOESN'T EXIST");
		}


		// List actors will be read into.
		std::list<Actor*> dynamicActors;

		try{
			// Create archive.
			iarchive_t i_archive{ i_stream };
			i_archive >> BOOST_SERIALIZATION_NVP(dynamicActors);

		}
		catch (boost::archive::archive_exception& e)
		{
			ENG_LOGLN(Fatal) << "ARCHIVE ERROR WHILE LOADING SAVED ACTORS! Reason: " << e.what() << " Error code: " << e.code;
		}
		catch (std::exception& e)
		{
			ENG_LOGLN(Fatal) << "ERROR ENCOUNTERED WHILE LOADING SAVED ACTORS! Reason: " << e.what();
		}

		// move actors to map.
		for (auto& elem : dynamicActors)
		{
			actors[static_cast<DefaultWorldLocation*>(elem->GUID)->location] = elem;
		}
	}
	ENG_LOGLN(Trace) << "Actors Loaded!";

	///////////////////////////
	// begin background loading
	{

		LOAD_PROPERTY_WITH_ERROR(propManager, "chunk.size", backgroundChunkSize);

		std::vector<uint8> data;
		uvec2 size = ImageLoader::load(folderLocation + name + "\\" + name + ".png", data);
		numBackgroundChunks = size / backgroundChunkSize;

		
		background = new ChunkActor*[numBackgroundChunks.x * numBackgroundChunks.y];

		std::vector<vec2> locations{ backgroundChunkSize * backgroundChunkSize * 4};
		std::vector<vec2> UVs{ backgroundChunkSize * backgroundChunkSize * 4};
		std::vector<uvec3> elems{ backgroundChunkSize * backgroundChunkSize * 2 };

		// generate location and elem data -- all the same
		for (uint16 yTiles = 0; yTiles < backgroundChunkSize; ++yTiles)
		{
			for (uint16 xTiles = 0; xTiles < backgroundChunkSize; ++xTiles)
			{
				uint32 startIndex = yTiles * backgroundChunkSize * 4 + xTiles * 4;

				locations[startIndex    ] = vec2(xTiles    , yTiles    );
				locations[startIndex + 1] = vec2(xTiles    , yTiles + 1);
				locations[startIndex + 2] = vec2(xTiles + 1, yTiles    );
				locations[startIndex + 3] = vec2(xTiles + 1, yTiles + 1);

				elems[yTiles * backgroundChunkSize * 2 + xTiles * 2    ] = uvec3(startIndex    , startIndex + 1, startIndex + 2);
				elems[yTiles * backgroundChunkSize * 2 + xTiles * 2 + 1] = uvec3(startIndex + 1, startIndex + 2, startIndex + 3);
			}
		}

		ModelBounds bounds{ 
			vec2(0.f, 0.f), 
			vec2(0.f, backgroundChunkSize),
			vec2(backgroundChunkSize, 0.f), 
			vec2(backgroundChunkSize, backgroundChunkSize) };

		// generate UV 
		for (uint16 yChunks = 0; yChunks < numBackgroundChunks.y; ++yChunks)
		{
			for (uint16 xChunks = 0; xChunks < numBackgroundChunks.x; ++xChunks)
			{
				for (uint16 yTiles = 0; yTiles < backgroundChunkSize; ++yTiles)
				{
					for (uint16 xTiles = 0; xTiles < backgroundChunkSize; ++xTiles)
					{

						uint32 startColIndex = ((numBackgroundChunks.y - yChunks - 1) * numBackgroundChunks.x + xChunks) * 4 * backgroundChunkSize * backgroundChunkSize + ((backgroundChunkSize - yTiles - 1) * backgroundChunkSize * 4) + (4 * xTiles);

						Color col = Color(
							data[startColIndex],
							data[startColIndex + 1],
							data[startColIndex + 2],
							data[startColIndex + 3]);

						auto imageIter = imageToTextureAssoc.find(col);

						// if it exists
						if (imageIter != imageToTextureAssoc.end())
						{
							std::string imageName = imageIter->second;


							QuadUVCoords coords = backgroundImages->getUVCoords(imageName);

							uint32 startTileIndex = (yTiles * backgroundChunkSize * 4) + (xTiles * 4);

							UVs[startTileIndex    ] = coords.lowerLeft;
							UVs[startTileIndex + 1] = coords.upperLeft;
							UVs[startTileIndex + 2] = coords.lowerRight;
							UVs[startTileIndex + 3] = coords.upperRight;
						}
						else
						{
							ENG_LOGLN(Warning) << "Warining: could not find image name in imageAssoc map in DefaultWorld";
						}


					}
				}

				
				
				background[yChunks * numBackgroundChunks.x * xChunks] = 
					new ChunkActor(
						Transform{
							vec2(xChunks * backgroundChunkSize, yChunks * backgroundChunkSize)
						},
						ModelData(
							&locations[0],
							&UVs[0], 
							&elems[0],
							backgroundChunkSize * backgroundChunkSize * 4,
							backgroundChunkSize * backgroundChunkSize * 2,
							drawMaterial,
							bounds
						));

			}
		}


	}
	// end background loading


	ENG_LOGLN(Trace) << "World Loaded!";
}

ActorLocation* DefaultWorld::addActor(Actor* toAdd)
{
	actors.insert({ nextIndex, toAdd });
	DefaultWorldLocation* loc = new DefaultWorldLocation(nextIndex);
	toAdd->GUID = loc;
	nextIndex++;

	return loc;
	
}

void DefaultWorld::save()
{
	ENG_LOGLN(Trace) << "Saving world";
	// Create list to save
	std::list<Actor*> toSave;
	for (auto& pair : actors) 
	{
		if (pair.second->needsSave()) 
		{
			toSave.push_back(pair.second);
		}
	} // End for

	// File location of static actors -- if we are binary, then use a binary stream
	std::ofstream o_stream{ folderLocation + worldName + '\\' + worldName + ".SAVE",
#	if IS_SAVE_BINARY
		std::ifstream::binary
#	endif
	};

	try{

		// Create archive.
		oarchive_t o_archive{ o_stream };
		// Save.
		o_archive << BOOST_SERIALIZATION_NVP(toSave);

	}
	catch (boost::archive::archive_exception& e)
	{
		ENG_LOGLN(Error) << "ARCHIVE ERROR SAVING ACTORS. Reason: " << e.what() << " Code: " << e.code;
	}
	catch (std::exception& e)
	{
		ENG_LOGLN(Error) << "ERROR SAVING ACTORS. Reason: " << e.what();
	}
}

void DefaultWorld::consoleCommand(std::string& command)
{
	string giveCommand;
	string args;
	try {
		commandMap[giveCommand](args);
	}
	catch (exception e) {
		ENG_LOGLN(Error) << "Error in command " << command << ": " << e.what();
		// We also may want to tell the UI it went wrong.
	}
}

PlayerController* DefaultWorld::makePlayerController()
{
	return nullptr; // TODO: fix
}

Pawn* DefaultWorld::makePawn()
{
	return nullptr; // TODO: fix
}