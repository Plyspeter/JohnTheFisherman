// Game Programming 2020
// Exam Project: John the Fisherman

#include "Enemy.hpp"
#include "Room.hpp"
#include "GameObject.hpp"
#include "SoundComponent.hpp"
#include "PhysicsComponent.hpp"
#include "PlayerController.hpp"

Enemy::Enemy(GameObject* gameObject) : Component(gameObject), Hookable(gameObject)
{
	b2Filter filter;
	filter.categoryBits = 0x0002;
	gameObject->getComponent<PhysicsComponent>()->getBody()->GetFixtureList()->SetFilterData(filter);
}

void Enemy::kill()
{
	dead = true;

	if (gameObject->hasComponent<SoundComponent>())
	{
		gameObject->getComponent<SoundComponent>()->play("death", 128);
	}

	gameObject->removeObject();
}

bool Enemy::isDead()
{
	return dead;
}

void Enemy::onCollisionStart(PhysicsComponent* component)
{
	if (component->getGameObject()->hasComponent<PlayerController>())
	{
		bool didHurt = component->getGameObject()->getComponent<PlayerController>()->hurt();
		if(didHurt) kill();
	}
}

void Enemy::read(rapidjson::Value& component)
{
	dead = component["dead"].GetBool();
}

void Enemy::write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a)
{
	rapidjson::Value componenType((int)Comp::EnemyC);
	c.AddMember("componentType", componenType, a);
	rapidjson::Value dead(dead);
	c.AddMember("dead", dead, a);
}
