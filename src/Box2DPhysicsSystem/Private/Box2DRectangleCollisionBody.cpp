#include "Box2DRectangleCollisionBody.h"

#include "Box2DPhysicsSystem.h"

Box2DRectangleCollisionBody::Box2DRectangleCollisionBody(RectangleCollisionComponent* owner, Box2DPhysicsSystem* system)
	:ownerComp(owner),
	physicsSystem(system)
{
	system->addBody(this);

	b2BodyDef bodyDef;
	bodyDef.type = b2BodyType::b2_dynamicBody;
	bodyDef.position = convertVec(ownerComp->getWorldLocation());
	bodyDef.angle = ownerComp->getWorldRotation();
	
	bodyDef.userData = ownerComp; // set the cusom user data the the ownerComp so we can get it later

	body = system->getWorld()->CreateBody(&bodyDef);

	b2FixtureDef fixDef;
	b2PolygonShape shape;
	shape.SetAsBox(100, 100); // TODO: actual width and height
	fixDef.shape = &shape;
	fixDef.density = 10000.f;
	// TODO: set other params

	fixture = body->CreateFixture(&fixDef);
	
	
}

Box2DRectangleCollisionBody::~Box2DRectangleCollisionBody()
{
	body->DestroyFixture(fixture);
	physicsSystem->getWorld()->DestroyBody(body);

}

void Box2DRectangleCollisionBody::update()
{
	ownerComp->getOwner()->setLocation(convertVec(body->GetPosition()));
	ownerComp->getOwner()->setRotation(body->GetAngle());

	
}