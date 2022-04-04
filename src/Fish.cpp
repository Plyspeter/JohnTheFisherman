// Game Programming 2020
// Exam Project: John the Fisherman

#include "Fish.hpp"
#include "GlobalMacros.hpp"
#include "GameObject.hpp"
#include "Room.hpp"

Fish::Fish(GameObject* gameObject) : Component(gameObject), Hookable(gameObject), CantEscapeRoom(gameObject)
{
}

void Fish::update(float deltaTime)
{
	CantEscapeRoom::update(deltaTime);
}

void Fish::read(rapidjson::Value& component)
{
}

void Fish::write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a)
{
	rapidjson::Value componenType((int)Comp::BarrelC);
	c.AddMember("componentType", componenType, a);

}
