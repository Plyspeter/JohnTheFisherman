// Game Programming 2020
// Exam Project: John the Fisherman
// Inspired by assignments from course

#pragma once
#include "sre/SpriteBatch.hpp"
#include "rapidjson/document.h"

class GameObject;
class PhysicsComponent;
class Room;

class Component {
public:
	explicit Component(GameObject* gameObject);
	virtual ~Component() = default;

	GameObject* getGameObject();

	virtual void onKey(SDL_Event& event);
	virtual void update(float deltaTime);
	virtual void renderSprite(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder);
	virtual void renderDrawing(sre::RenderPass* renderPass);
	virtual void onGui();
	virtual void onCollisionStart(PhysicsComponent* comp);
	virtual void onCollisionEnd(PhysicsComponent* comp);
	virtual void read(rapidjson::Value& component);
	virtual void write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a);

protected:
	GameObject* gameObject;

	friend class GameObject;
};