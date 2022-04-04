// Game Programming 2020
// Exam Project: John the Fisherman

#include "Barrel.hpp"
#include "GlobalMacros.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "Room.hpp"

Barrel::Barrel(GameObject* gameObject) : Component(gameObject), Hookable(gameObject), CantEscapeRoom(gameObject)
{
	ASSERT(gameObject->getComponent<PhysicsComponent>().get() != nullptr, "PlayerController component missing PhysicsComponent in gameobject: " + gameObject->getName());
	ASSERT(gameObject->getComponent<SpriteComponent>().get() != nullptr, "PlayerController component missing SpriteComponent in gameobject: " + gameObject->getName());
}

void Barrel::update(float deltaTime)
{
	CantEscapeRoom::update(deltaTime);
}

void Barrel::read(rapidjson::Value& component)
{
}

void Barrel::write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a)
{
	rapidjson::Value componenType((int)Comp::BarrelC);
	c.AddMember("componentType", componenType, a);

}
