// Game Programming 2020
// Exam Project: John the Fisherman
// Inspired by assignments from course

#include "GameObject.hpp"

int GameObject::amount = 0;

GameObject::GameObject()
{
	GameObject::amount++;
}

GameObject::~GameObject()
{
	GameObject::amount--;
	for (auto& comp : components)
		comp->gameObject = nullptr;
}

void GameObject::removeComponent(std::shared_ptr<Component> component)
{
	auto comp = find(components.begin(), components.end(), component);

	if (comp != components.end())
	{
		components.erase(comp);
	}
}

void GameObject::update(float deltaTime)
{
	for (auto& comp : components) 
	{
		comp->update(deltaTime);
	}
}

void GameObject::render(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder)
{
	for (auto& comp : components)
	{
		comp->renderSprite(spriteBatchBuilder);
	}
}

void GameObject::renderDrawing(sre::RenderPass* renderPass) {
	for (auto& comp : components)
	{
		comp->renderDrawing(renderPass);
		comp->onGui();
	}
}

void GameObject::setPosition(const glm::vec2& position)
{
	GameObject::position = position;
	positionSet = true;
}

const glm::vec2& GameObject::getPosition() const {
	return position;
}

const std::vector<std::shared_ptr<Component>>& GameObject::getComponents()
{
	return components;
}

std::string GameObject::getName()
{
	return name;
}

void GameObject::setName(std::string name)
{
	this->name = name;
}

bool GameObject::positionIsSet()
{
	return positionSet;
}

void GameObject::removeObject()
{
	this->deleteObject = true;
}

bool GameObject::shouldRemoveObject()
{
	return this->deleteObject;
}