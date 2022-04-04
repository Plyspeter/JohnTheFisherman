// Game Programming 2020
// Exam Project: John the Fisherman

#include "MovingPlatform.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "Room.hpp"
#include "GlobalMacros.hpp"

MovingPlatform::MovingPlatform(GameObject* gameObject)
	: Component(gameObject)
{
	ASSERT(gameObject->positionIsSet(), "MovingPlatform component added before position was set in gameobject: " + gameObject->getName());
	ASSERT(gameObject->getComponent<PhysicsComponent>().get() != nullptr, "MovingPlatform missing PhysicsComponent in gameobject: " + gameObject->getName());
	start = gameObject->getPosition();
}

void MovingPlatform::setEnd(glm::vec2 relativePosition)
{
	relativeEnd = relativePosition;
	end = start + relativePosition;
}

void MovingPlatform::setSpeed(float speed)
{
	this->speed = speed;
}

void MovingPlatform::update(float deltaTime)
{
	total += deltaTime;

	if (total > speed) {
		total -= speed;
		reverse = !reverse;
	}

	auto x = glm::smoothstep(0.0f, speed, total);
	glm::vec2 pos;

	if (reverse) 
	{
		pos = glm::mix(start, end, x);
	}
	else 
	{
		pos = glm::mix(end, start, x);
	}

	gameObject->setPosition(pos);
	auto phys = gameObject->getComponent<PhysicsComponent>();

	phys->moveTo(pos);
}

void MovingPlatform::read(rapidjson::Value& component)
{
	reverse = component["reverse"].GetBool();
	if(component.HasMember("total")) total = component["total"].GetFloat();
	speed = component["speed"].GetFloat();
	if (component.HasMember("start"))
		start = { component["start"]["x"].GetFloat(), component["start"]["y"].GetFloat() };
	setEnd(glm::vec2(component["relativeEnd"]["x"].GetFloat(), component["relativeEnd"]["y"].GetFloat()));
}

void MovingPlatform::write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a)
{
	rapidjson::Value componenType((int)Comp::MovingPlatformC);
	c.AddMember("componentType", componenType, a);
	rapidjson::Value reverse(reverse);
	c.AddMember("reverse", reverse, a);
	rapidjson::Value total(total);
	c.AddMember("total", total, a);
	rapidjson::Value speed(speed);
	c.AddMember("speed", speed, a);
	
	rapidjson::Value jStart(rapidjson::kObjectType);
	{
		rapidjson::Value x(start.x);
		jStart.AddMember("x", x, a);
		rapidjson::Value y(start.y);
		jStart.AddMember("y", y, a);
	}
	c.AddMember("start", jStart, a);
	rapidjson::Value jEnd(rapidjson::kObjectType);
	{
		rapidjson::Value x(relativeEnd.x);
		jEnd.AddMember("x", x, a);
		rapidjson::Value y(relativeEnd.y);
		jEnd.AddMember("y", y, a);
	}
	c.AddMember("relativeEnd", jEnd, a);
}
