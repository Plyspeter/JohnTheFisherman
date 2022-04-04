// Game Programming 2020
// Exam Project: John the Fisherman
// Inspired by assignments from course

#pragma once
#include "PhysicsComponent.hpp"
#include "sre/SDLRenderer.hpp"
#include "Box2DDebugDraw.hpp"
#include "Box2D/Dynamics/b2World.h"
#include "Box2D/Dynamics/Contacts/b2Contact.h"

class PhysicsEngine : public b2ContactListener
{
public:
	PhysicsEngine();
	~PhysicsEngine();

	void registerPhysicsComponent(PhysicsComponent* component);
	void deregisterPhysicsComponent(PhysicsComponent* component);

	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;

	void swapDebugDraw();
	void renderDebug(sre::RenderPass* renderPass);

	static PhysicsEngine* instance;

	static constexpr float physicsScale = 100;
	static constexpr float timeStep = 1.0f / 60.0f;

private:
	const float gravity = -9.8;
	const int velocityIterations = 6;
	const int positionIterations = 2;

	Box2DDebugDraw debugDraw;
	bool doDebugDraw = false;

	b2World* world = nullptr;

	std::map<b2Fixture*, PhysicsComponent*> physicsComponents;

	void update();

	void handleContact(b2Contact* contact, bool begin);

	friend class FishermanGame;
	friend class PhysicsComponent;
	friend class PlayerController;
};