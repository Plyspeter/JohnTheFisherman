// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "Knife.hpp"
#include "Enemy.hpp"
#include "FishermanGame.hpp"
#include "PlayerController.hpp"
#include "SpriteComponent.hpp"

Knife::Knife(GameObject* gameObject)
	: Component(gameObject)
{
	gameObject->setName("knife");

	auto player = FishermanGame::instance->getPlayer();
	if (player->getComponent<PlayerController>()->isMovingRight())
		gameObject->setPosition({ player->getPosition().x + 25, player->getPosition().y });
	else
		gameObject->setPosition({ player->getPosition().x - 25, player->getPosition().y });

	auto phys = gameObject->addComponent<PhysicsComponent>();

	phys->initBox(b2_kinematicBody, { 10, 18}, 1);
	phys->setSensor();
}

void Knife::update(float deltaTime)
{
	auto player = FishermanGame::instance->getPlayer();
	if (!player->getComponent<SpriteComponent>()->getFlip())
		gameObject->setPosition({ player->getPosition().x + 25, player->getPosition().y });
	else
		gameObject->setPosition({ player->getPosition().x - 25, player->getPosition().y });

	auto phys = gameObject->getComponent<PhysicsComponent>();

	phys->moveTo(gameObject->getPosition());

	timer -= deltaTime;
	// Remove the knife when the time is up.
	if (timer < 0.0f)
	{
		gameObject->removeObject();
	}
}

void Knife::onCollisionStart(PhysicsComponent* component)
{
	// If the knife hits an enemy, destory the enemy
	if (component->getGameObject()->hasComponent<Enemy>())
	{
		component->getGameObject()->removeObject();
	}
}