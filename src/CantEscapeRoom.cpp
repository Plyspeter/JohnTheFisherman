// Game Programming 2020
// Exam Project: John the Fisherman

#include "CantEscapeRoom.hpp"
#include "PhysicsComponent.hpp"
#include "FishermanGame.hpp"

CantEscapeRoom::CantEscapeRoom(GameObject* gameObject)
{
	cesGameObject = gameObject;
}

void CantEscapeRoom::update(float deltaTime)
{
	auto phys = cesGameObject->getComponent<PhysicsComponent>();
	auto left = FishermanGame::instance->getLeftScreenCoord();
	auto right = FishermanGame::instance->getRightScreenCoord();
	auto top = FishermanGame::instance->getTopScreenCoord();
	auto bottom = FishermanGame::instance->getBottomScreenCoord();
	if (cesGameObject->getPosition().x - phys->getSize().x < left)
		phys->setLinearVelocity({ 1,0 });
	if (cesGameObject->getPosition().x + phys->getSize().x > right)
		phys->setLinearVelocity({ -1,0 });
}