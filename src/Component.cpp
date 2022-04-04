// Game Programming 2020
// Exam Project: John the Fisherman
// Inspired by assignments from course

#include "Component.hpp"

Component::Component(GameObject* gameObject)
	:gameObject(gameObject)
{

}

void Component::update(float deltaTime)
{

}

void Component::renderSprite(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder)
{

}

void Component::onKey(SDL_Event& event)
{

}

void Component::renderDrawing(sre::RenderPass* renderPass)
{

}

void Component::onGui()
{

}

void Component::onCollisionStart(PhysicsComponent* comp)
{

}

void Component::onCollisionEnd(PhysicsComponent* comp) 
{

}

void Component::read(rapidjson::Value& component)
{
	throw "Baseclass \"Component\" received call to method \"read\"";
}

void Component::write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a)
{
	throw "Baseclass \"Component\" received call to method \"write\"";
}

GameObject* Component::getGameObject()
{
	return gameObject;
}