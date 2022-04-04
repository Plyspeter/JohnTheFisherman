// Game Programming 2020
// Exam project: John the Fisherman

#pragma once
#include "Enemy.hpp"
#include "GameObject.hpp"

class EnemyRunner : public Enemy
{
public:
	explicit EnemyRunner(GameObject* gameObject);

	void update(float deltaTime) override;

	void read(rapidjson::Value& component) override;
	void write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a) override;

private:
	float acc = 0.03f;
	float maxVelocity = 1.2f;
	float soundTimer = 0.4; //Timer for when to play sound
};