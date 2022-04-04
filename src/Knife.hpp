// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "Component.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"


class Knife : public Component
{
public:
	explicit Knife(GameObject* gameObject);

	void update(float deltaTime);

	void onCollisionStart(PhysicsComponent* component) override;

private:
	float timer = 0.5f;
};