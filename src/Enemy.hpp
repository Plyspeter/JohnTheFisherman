// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "Component.hpp"
#include "Hookable.hpp"

class Enemy : public Component, public Hookable 
{
public:
	explicit Enemy(GameObject* gameObject);

	void kill();
	bool isDead();
	void onCollisionStart(PhysicsComponent* component) override;
	void read(rapidjson::Value& component) override;
	void write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a) override;

protected:
	bool dead = false;
};