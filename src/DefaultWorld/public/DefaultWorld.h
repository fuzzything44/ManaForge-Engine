#pragma once
#include "DefaultWorldConfig.h"
#include <ENGException.h>
#include <World.h>
#include <PropertyManager.h>
#include <Runtime.h>
#include <Actor.h>
#include <functional>
#include <TextureLibrary.h>
#include <deque>

#include <boost/signals2.hpp>

#include <array>

#include "ChunkActor.h"

#include <Color.h>


class DefaultWorld : public World
{
public:

	friend struct DefaultWorldLocation;

	explicit DefaultWorld(const std::string& name = "");
	virtual ~DefaultWorld();


	bool update(float deltaTime);

	// World Interface
	virtual void init(const std::string& name) override;

	virtual void saveWorld() override;

	virtual std::unique_ptr<ActorLocation> addActor(Actor& toAdd) override;

	virtual PlayerController* makePlayerController() override;
	virtual Pawn* makePawn() override;

	virtual boost::signals2::connection registerTickingActor(Actor& toAdd) override;
	// End World Interface
	typedef std::map<uint64, Actor*>::iterator iter_type;
private:

	std::string folderLocation;
	std::string worldName;

	PropertyManager propManager; // for world specific properties

	// use a deque -- the index can be the index in it!
	std::deque<Actor*> actors;

	std::unique_ptr<TextureLibrary> backgroundImages;
	std::shared_ptr<Material> drawMaterial;

	// array of models -- in row major order
	std::unique_ptr<ChunkActor*[]> background;

	uvec2 numBackgroundChunks;

	uint32 backgroundChunkSize;

	std::string playerControllerClassName;
	std::string pawnClassName;

	boost::signals2::signal<void(float)> tickingActors;

};
