// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "Enemy.hpp"
#include "GameObject.hpp"

class EnemyFlyer : public Enemy
{
public:
	explicit EnemyFlyer(GameObject* gameObject);

	void update(float deltaTime) override;
	void onCollisionStart(PhysicsComponent* comp) override;

	void read(rapidjson::Value& component) override;
	void write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a) override;

private:
	const glm::vec2 maxVelocity = {1.2f, 1.2f};
	glm::vec2 direction;
	float soundTimer = 3; //Timer to count when to play sound
};