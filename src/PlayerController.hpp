// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "Component.hpp"
#include "GameObject.hpp"
#include "Hook.hpp"
#include "sre/Texture.hpp"
#include "Box2D/Box2D.h"

class PlayerController : public Component, public b2RayCastCallback
{
public:
	explicit PlayerController(GameObject* gameObject);

	void update(float deltaTime) override;
	void onKey(SDL_Event& event) override;
	void onCollisionStart(PhysicsComponent* component) override;
	virtual float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) override;
	void renderDrawing(sre::RenderPass* renderPass) override;
	void onGui() override;

	std::shared_ptr<GameObject> getHook();
	bool isMovingRight();
	bool hurt();
	void resetPosition();

	static const glm::vec2 PlayerController::initialPosition;

private:
	static std::shared_ptr<sre::Texture> PlayerController::lifeTex;

	const float acc = 0.01f;
	const float maxVelocity = 2;
	const float jumpHeight = 0.7f;
	int numOfLifes = 3;

	bool isGrounded = false;
	bool left = false;
	bool right = false;
	bool movingRight = true;
	bool hooking = false;
	bool attacking = false;
	bool hurting = false;

	// Boolean values used to prevent holding down keys
	bool hookup = true;
	bool jumpup = true;

	float attackTime = 0.5f;
	float hurtTime = 1.5f;

	std::shared_ptr<GameObject> hook = nullptr;

	void jump();
	void checkIfGrounded();
	void throwHook();
	void attack();
};