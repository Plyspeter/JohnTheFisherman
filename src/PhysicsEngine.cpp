// Game Programming 2020
// Exam Project: John the Fisherman
// Inspired by assignments from course

#pragma once
#include "PhysicsEngine.hpp"
#include "GameObject.hpp"

PhysicsEngine* PhysicsEngine::instance = nullptr;

PhysicsEngine::PhysicsEngine()
	:debugDraw(physicsScale)
{
	instance = this;

	delete world;

	world = new b2World(b2Vec2(0, gravity));
	world->SetContactListener(this);

	physicsComponents.clear();

	if (doDebugDraw)
	{
		world->SetDebugDraw(&debugDraw);
	}
}

PhysicsEngine::~PhysicsEngine()
{
	instance = nullptr;
	physicsComponents.clear();
	delete world;
}

void PhysicsEngine::update()
{
	world->Step(timeStep, velocityIterations, positionIterations);

	for (auto component : physicsComponents)
	{
		if (component.second->bodyType == b2_staticBody)
		{
			continue;
		}

		b2Vec2 position = component.second->body->GetPosition();
		GameObject* gameObject = component.second->getGameObject();

		gameObject->setPosition(glm::vec2(position.x * physicsScale, position.y * physicsScale));
	}
}

void PhysicsEngine::swapDebugDraw()
{
	doDebugDraw = !doDebugDraw;

	if (doDebugDraw)
	{
		world->SetDebugDraw(&debugDraw);
	}
	else
	{
		world->SetDebugDraw(nullptr);
	}
}

void PhysicsEngine::renderDebug(sre::RenderPass* renderPass)
{
	if (doDebugDraw)
	{
		world->DrawDebugData();
		renderPass->drawLines(debugDraw.getLines());
		debugDraw.clear();
	}
}

void PhysicsEngine::registerPhysicsComponent(PhysicsComponent* component)
{
	physicsComponents[component->fixture] = component;
}

void PhysicsEngine::deregisterPhysicsComponent(PhysicsComponent* component)
{
	auto comp = physicsComponents.find(component->fixture);

	if (comp != physicsComponents.end())
	{
		physicsComponents.erase(comp);
	}
}

void PhysicsEngine::BeginContact(b2Contact* contact)
{
	b2ContactListener::BeginContact(contact);
	handleContact(contact, true);
}

void PhysicsEngine::EndContact(b2Contact* contact)
{
	b2ContactListener::EndContact(contact);
	handleContact(contact, false);
}

void PhysicsEngine::handleContact(b2Contact* contact, bool begin)
{
	auto physA = physicsComponents.find(contact->GetFixtureA());
	auto physB = physicsComponents.find(contact->GetFixtureB());

	if (physA == physicsComponents.end() || physB == physicsComponents.end())
		return;

	for (auto& component : physA->second->getGameObject()->getComponents())
	{
		if (begin)
			component->onCollisionStart(physB->second);
		else
			component->onCollisionEnd(physB->second);
	}

	for (auto& component : physB->second->getGameObject()->getComponents())
	{
		if (begin)
			component->onCollisionStart(physA->second);
		else
			component->onCollisionEnd(physA->second);
	}
}