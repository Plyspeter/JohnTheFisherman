// Game Programming 2020
// Exam Project: John the Fisherman

#include "EnemyRunner.hpp"
#include "Room.hpp"
#include "GlobalMacros.hpp"
#include "SoundComponent.hpp"
#include "FishermanGame.hpp"

EnemyRunner::EnemyRunner(GameObject* gameObject) : Enemy(gameObject)
{
	ASSERT(gameObject->getComponent<PhysicsComponent>().get() != nullptr, "EnemyRunner component missing PhysicsComponent in gameobject: " + gameObject->getName());
}

void EnemyRunner::update(float deltaTime)
{
	if (dead)
		return;

	auto phys = gameObject->getComponent<PhysicsComponent>();
	auto player = FishermanGame::instance->getPlayer();

	glm::vec2 movement{ 0, 0 };

	if (gameObject->getPosition().x > player->getPosition().x)
	{
		movement.x--;
	}
	else if (gameObject->getPosition().x < player->getPosition().x)
	{
		movement.x++;
	}

	phys->addImpulse(movement * acc);

	auto linearVelocity = phys->getLinearVelocity();
	float currentVelocity = linearVelocity.x;

	if (abs(currentVelocity) > maxVelocity)
	{
		linearVelocity.x = glm::sign(linearVelocity.x) * maxVelocity;
		phys->setLinearVelocity(linearVelocity);
	}

	soundTimer += deltaTime;
	if (gameObject->hasComponent<SoundComponent>() && soundTimer >= 1)
	{
		soundTimer = 0;
		gameObject->getComponent<SoundComponent>()->play("movement", 128);
	}
}

void EnemyRunner::read(rapidjson::Value& component)
{
	Enemy::read(component);
	acc = component["acc"].GetFloat();
	maxVelocity = component["maxVelocity"].GetFloat();
}

void EnemyRunner::write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a)
{
	Enemy::write(c, a);
	c["componentType"].SetInt((int)Comp::SkeletonRunnerC);
	rapidjson::Value jAcc(acc);
	c.AddMember("acc", jAcc, a);
	rapidjson::Value jMaxVel(maxVelocity);
	c.AddMember("maxVelocity", jMaxVel, a);
}
