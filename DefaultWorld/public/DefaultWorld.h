#include "DefaultWorldConfig.h"
#include <ENGException.h>
#include <World.h>
#include <PropertyManager.h>

class DefaultWorld : public World
{
private:
	std::string folderLocation;

	Chunk* persistentMain;
	Chunk*** mainWorld;

	Chunk* persistentCurrent;
	Chunk*** currentWorld;

	uint32 chunkSize;

	uvec2 numChunks;

	PropertyManager propManager;

public:
	explicit DefaultWorld(std::string folder = ""); 

	virtual void loadWorld(std::string name) override;


	virtual ~DefaultWorld();
};
