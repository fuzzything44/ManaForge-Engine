#pragma once
#include "Box2DPhysicsSystemConfig.h"

#include <PhysicsSystem.h>
#include <Actor.h>

#include <map>

class PhysicsComponent;
class Box2DActorTransformController;

class Box2DPhysicsSystem : public PhysicsSystem
{
	friend class Box2DPhysicsBody;
	friend Box2DActorTransformController;
public:

	Box2DPhysicsSystem();
	virtual ~Box2DPhysicsSystem();

	virtual void setGravity(vec2 newGravity) override;


	virtual PhysicsBody* newPhysicsBody(PhysicsShape& shape, PhysicsComponent& owner) override;
	virtual std::unique_ptr<PhysicsShape> newPhysicsShape() override;
	virtual ActorTransformController* newActorTransformController(Actor& actor) override;
	virtual void drawDebugPoints() override;

	bool update(float deltaTime);


private:
	
	std::map<Actor*, Box2DActorTransformController*> bodies;
	
	b2World* world;
	b2Vec2 gravity;
};
