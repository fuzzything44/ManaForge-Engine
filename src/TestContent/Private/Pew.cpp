#include "Pew.h"
#include "Gate.h"

#include <sstream>
#include <locale>
#include <string>
#include <chrono>

#include <TimerManager.h>

#include <boost/convert.hpp>
#include <boost/convert/lexical_cast.hpp>

MFCLASS_SOURCE(Pew)

Pew::Pew()
	: timeToSpawn(0.f)
{
	auto shape = Runtime::get().getPhysicsSystem().newPhysicsShape();
	shape->asRectangle(2.f, 2.f);

	phys = std::make_unique<PhysicsComponent>(*this, *shape);
	phys->setStartContactCallback([this](PhysicsComponent& other)
		{
			startContact(other);
		});

	textBox = Runtime::get().getRenderer().newTextBox();
	textBox->setFont(Runtime::get().getRenderer().getFont("Arial"));
	textBox->setText(u"Contacts: 0");
	textBox->setLocation({.5f, .4f});
	textBox->setColor({.3f, 0.f, 1.f, 1.f});

	Runtime::get().getTimerManager().addTimer(TimerManager::Double_duration_t(1.f),
		[]
		{
			auto g = new Gate();
			g->setVelocity({5.f, 5.f});
			g->setWorldLocation({3.f, 3.f});
		},
		true);
}

void Pew::startContact(PhysicsComponent& other)
{
	using namespace std::string_literals;

	textBox->setText(u"Contacts: "s + reinterpret_cast<const char16_t*>(std::to_wstring(++contacts).c_str()));

	if (reinterpret_cast<Pawn*>(&other.getOwner()) == Runtime::get().pawn.get()) {
		for (int i = 0; i < 100; ++i) {
			auto g = new Gate();
			g->setWorldLocation(getWorldLocation() + vec2(2.f, 1.f));
			g->setVelocity(vec2(1.f, .3f));

			// not a memory leak: Gates clean theirselvs up.
		}
	}
}
