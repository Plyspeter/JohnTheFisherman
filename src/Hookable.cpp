// Game Programming 2020
// Exam Project: John the Fisherman

#include "Hookable.hpp"
#include "PhysicsComponent.hpp"

Hookable::Hookable(GameObject* go)
{
	hookableGameObject = go;
}

bool Hookable::isHooked() {return hooked;}
bool Hookable::isReturning() {return returning;}

void Hookable::returnWithHook()
{
	returning = true;
	auto phys = hookableGameObject->getComponent<PhysicsComponent>();
	if (phys.get() != nullptr)
	{
		b2MassData* massData = &b2MassData();
		phys->getBody()->GetMassData(massData);
		massData->mass = 0.001;
		phys->getBody()->SetMassData(massData);
	}
}

void Hookable::hook() {hooked = true;}

void Hookable::unHook()
{
	hooked = false;
	auto phys = hookableGameObject->getComponent<PhysicsComponent>();
	if (phys.get() != nullptr)
		phys->getBody()->ResetMassData();
}
