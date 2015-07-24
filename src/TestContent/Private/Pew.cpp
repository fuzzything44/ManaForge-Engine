#include "Pew.h"
#include "Gate.h"


MFCLASS_SOURCE(Pew)

Pew::Pew ()
	: timeToSpawn(0.f)
{
	auto shape = Runtime::get().physSystem->newPhysicsShape();
	shape->asRectangle(2.f, 2.f);

	phys = std::make_unique<PhysicsComponent>(*this, *shape);
	phys->setStartContactCallback([this](PhysicsComponent& other)
	{
		startContact(other);
	});
}

void Pew::tick(float deltaTime)
{

	
}

void Pew::startContact(PhysicsComponent & other)
{
	
	if (&other.getOwner() == reinterpret_cast<Actor*>(Runtime::get().pawn.get()))
	{
		for (int i = 0; i < 100; ++i)
		{
			auto g = new Gate();
			g->setWorldLocation(getWorldLocation() + vec2(2.f, 1.f));
			g->setVelocity(vec2(1.f, .3f));
			// not a memory leak: Gates clean theirselvs up.
		}
	}
}
