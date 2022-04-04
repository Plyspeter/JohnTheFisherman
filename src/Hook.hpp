// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "Component.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "Hookable.hpp"


class Hook : public Component
{
public:
	explicit Hook(GameObject* gameObject);
	~Hook();

	void returnHook();
	void update(float deltaTime);
	void onCollisionStart(PhysicsComponent* component) override;
	bool isReturning();

private:
	// Impulse values for throwing the hook
	const float impulseX = 1.5f;
	const float impulseY = 1.4f;

	// Impulse Values for returning the hook
	const float minX = 0.021f;
	const float minY = 0.033f;

	bool returning = false;
	bool hooking = false;
	bool reReturn = false;

	std::shared_ptr<Hookable> hookedObj = nullptr;

	float calculateYImpulse(float angle, glm::vec2 playerPos);
	float calculateXImpulse(float angle, glm::vec2 playerPos);
};