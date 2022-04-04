// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "Hook.hpp"
#include "PhysicsEngine.hpp"
#include "FishermanGame.hpp"
#include "PlayerController.hpp"
#include "Enemy.hpp"

Hook::Hook(GameObject* gameObject) : Component(gameObject)
{
	//GameObject
	gameObject->setName("hook");
	auto player = FishermanGame::instance->getPlayer();

	if (player->getComponent<PlayerController>()->isMovingRight())
		gameObject->setPosition({ player->getPosition().x + 15, player->getPosition().y });
	else
		gameObject->setPosition({ player->getPosition().x - 15, player->getPosition().y });

	//Physics
	auto phys = gameObject->addComponent<PhysicsComponent>();
	phys->initCircle(b2_dynamicBody, 4, 1);
	phys->fixRotation(true);

	auto scale = PhysicsEngine::instance->physicsScale;
	auto pVel = player->getComponent<PhysicsComponent>()->getBody()->GetLinearVelocity();

	if (player->getComponent<PlayerController>()->isMovingRight())
		phys->addImpulse({ impulseX / scale, impulseY / scale });
	else
		phys->addImpulse({ -impulseX / scale, impulseY / scale });

	auto hVel = phys->getBody()->GetLinearVelocity();
	phys->setLinearVelocity({ (hVel.x + pVel.x), (hVel.y + pVel.y)});
	phys->addRopeJoint(player->getComponent<PhysicsComponent>().get(), b2Vec2(0, 0), b2Vec2(0, 0), 290);
}

Hook::~Hook()
{
	if (hookedObj.get() != nullptr)
		hookedObj->unHook();
}

void Hook::returnHook()
{
	auto player = FishermanGame::instance->getPlayer();

	// Calculate the return point for angle calculations
	float x = (player->getPosition().x + gameObject->getPosition().x) / 2;
	float y = player->getPosition().y + 40;
	glm::vec2 point = { x, y };

	// Calculate the angle that the hook needs to return at
	float AB = glm::abs(glm::distance(point, gameObject->getPosition()));
	float BC = glm::abs(glm::distance(point, glm::vec2{ point.x, gameObject->getPosition().y }));
	float angle = glm::asin(BC / AB);

	// Calculate the impulse in both 2D directions
	float impY = calculateYImpulse(angle, player->getPosition());
	float impX = calculateXImpulse(angle, player->getPosition());

	auto phys = gameObject->getComponent<PhysicsComponent>();

	// Reset the current velocity of the hook, so the it does not interfer with the new impulse.
	phys->setLinearVelocity({ 0, 0 });

	// Add the impulse to the hook object
	phys->addImpulse({ impX, impY });

	// If an object is hooked on the hook, also add impulse to it.
	if (hookedObj.get() != nullptr) hookedObj->returnWithHook();

	// Mark the hook as returning for later removal.
	returning = true;
}

float Hook::calculateYImpulse(float angle, glm::vec2 playerPos)
{
	// Calculation of the impule direction depended on the angle
	float base = glm::sin(angle);

	// Calculate the height difference between the player and the hook
	float distance = playerPos.y - gameObject->getPosition().y;

	// Calculate the multiplyer added, to make up for the height difference
	float multi = minY + ((distance - 23) / 10 * 0.00065f);

	return base * multi;
}

float Hook::calculateXImpulse(float angle, glm::vec2 playerPos)
{
	// Calculation of the impulse direction depended on the angle
	float base = glm::cos(angle);

	// Calculate the distance between the player and the hook
	float distance = playerPos.x - gameObject->getPosition().x;

	// Calculate the multiplyer added, to make up for the distance
	float multi = minX + ((glm::abs(distance) - 200) / 10 * 0.0004f);

	// Negate the value depending on the player direction
	return (distance < 0) ? -(base * multi) : (base * multi);
}

void Hook::update(float deltaTime)
{
	// If hook falls out of the map, remove it.
	if (gameObject->getPosition().y < -100)
		gameObject->removeObject();

	// If we hit object on way back, we return anew
	if (reReturn)
	{
		reReturn = false;
		returnHook();
	}

	//If hookedObj is only pointed to by us, it has been deleted and then so should the hook
	if (hooking && hookedObj.unique())
	{
		gameObject->removeObject();
	}

	// Only remove the hook if it is returning to the player
	if (returning)
	{
		auto velocity = gameObject->getComponent<PhysicsComponent>()->getLinearVelocity();

		// Remove hook if it does not move anymore
		if (abs(velocity.x) < 1 && abs(velocity.y) < 1)
		{
			gameObject->removeObject();
		}
	}
}

void Hook::onCollisionStart(PhysicsComponent* component)
{
	// Collisions done when not hooked
	if (!hooking)
	{
		// Collision with something that is not the player
		if (!component->getGameObject()->hasComponent<PlayerController>())
		{
			// When the hook hits an object, the hook should stop moving
			gameObject->getComponent<PhysicsComponent>()->setLinearVelocity({ 0, 0 });
			hookedObj = component->getGameObject()->getComponent<Hookable>();
			// If the collision is with an enemy, make the hook hook that enemy :P
			if (hookedObj.get() != nullptr)
			{
				auto phys = gameObject->getComponent<PhysicsComponent>();
				auto body = phys->getBody();
				phys->addRevoluteJoint(component, b2Vec2(0,0), b2Vec2(0,0));
				hooking = true;
				if (returning)
				{
					reReturn = true;
				}
			}
		}
	}

	// Collisions done when returning when not hooked
	if (returning && !hooking)
	{
		gameObject->removeObject();
	}
}

bool Hook::isReturning() {return returning;}