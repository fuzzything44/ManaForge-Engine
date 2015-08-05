#pragma once
#include "Box2DPhysicsSystemConfig.h"
#include "Box2DContactListener.h"

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

	virtual std::unique_ptr<PhysicsBody> newPhysicsBody(
		PhysicsShape& shape, PhysicsComponent& owner) override;
	virtual std::unique_ptr<PhysicsShape> newPhysicsShape() override;
	virtual std::unique_ptr<ActorTransformController> newActorTransformController(Actor& actor) override;
	virtual void drawDebugPoints() override;

	bool update(float deltaTime);

  private:
	std::unordered_map<Actor*, Box2DActorTransformController*> bodies;
	Box2DContactListener listener;

	std::unique_ptr<b2World> world;
	b2Vec2 gravity;
};
