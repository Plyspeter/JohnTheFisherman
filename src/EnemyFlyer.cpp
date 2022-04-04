// Game Programming 2020
// Exam Project: John the Fisherman

#include "EnemyFlyer.hpp"
#include "Room.hpp"
#include "GlobalMacros.hpp"
#include "PhysicsComponent.hpp"
#include "Box2D/Box2D.h"
#include "FishermanGame.hpp"
#include <ctime>
#include "SpriteComponent.hpp"
#include "SoundComponent.hpp"

EnemyFlyer::EnemyFlyer(GameObject* gameObject)
	:Enemy(gameObject)
{
	ASSERT(gameObject->getComponent<PhysicsComponent>().get() != nullptr, "EnemyFlyer component missing PhysicsComponent in gameobject: " + gameObject->getName());
	srand(time(nullptr)); //Seed rand to get new random value on each run
	direction = glm::vec2(rand() % 2 == 1 ? 1 : -1, rand() % 2 == 1 ? 1 : -1); //Set random direction to begin with
}

void EnemyFlyer::update(float deltaTime)
{
	if (dead)
		return;

	if (Hookable::isReturning())
	{
		return;
	}

	auto phys = gameObject->getComponent<PhysicsComponent>();

	auto left = FishermanGame::instance->getLeftScreenCoord();
	auto right = FishermanGame::instance->getRightScreenCoord();
	auto top = FishermanGame::instance->getTopScreenCoord();
	auto bottom = FishermanGame::instance->getBottomScreenCoord();
	if (gameObject->getPosition().x - phys->getRadius() < left || gameObject->getPosition().x + phys->getRadius() > right) //Reached side of room
		direction.x *= -1; //flip x direction
	if (gameObject->getPosition().y - phys->getRadius() < bottom || gameObject->getPosition().y + phys->getRadius() > top) //Reached top/bottom of room
		direction.y *= -1; //flip y direction

	if (direction.x < 0) {
		gameObject->getComponent<SpriteComponent>()->setFlip(true);
	}
	else {
		gameObject->getComponent<SpriteComponent>()->setFlip(false);
	}
	phys->setLinearVelocity(direction);

	soundTimer += deltaTime;
	if (gameObject->hasComponent<SoundComponent>() && soundTimer >= 4)
	{
		soundTimer = 0;
		gameObject->getComponent<SoundComponent>()->play("movement", 50);
	}
}

void EnemyFlyer::onCollisionStart(PhysicsComponent* comp)
{
	Enemy::onCollisionStart(comp);
	if (comp->getGameObject()->getName() == "hook")
		hooked = true;

	auto normal = comp->getCollisionNormal();
	direction = glm::reflect(direction, normal);
}

void EnemyFlyer::read(rapidjson::Value& component)
{
	Enemy::read(component);
	if(component.HasMember("direction"))
		direction = { component["direction"]["x"].GetFloat(), component["direction"]["y"].GetFloat() };
}

void EnemyFlyer::write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a)
{
	Enemy::write(c, a);
	c["componentType"].SetInt((int)Comp::EnemyFlyerC);

	rapidjson::Value dir(rapidjson::kObjectType);
	{
		rapidjson::Value x(direction.x);
		dir.AddMember("x", x, a);
		rapidjson::Value y(direction.y);
		dir.AddMember("y", y, a);
	}
	c.AddMember("direction", dir, a);
}
