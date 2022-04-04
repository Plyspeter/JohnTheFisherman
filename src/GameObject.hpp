// Game Programming 2020
// Exam Project: John the Fisherman
// Inspired by assignments from course

#pragma once
#include "Component.hpp"

class GameObject {
public:
	GameObject();
	~GameObject();

	template <class T>
	std::shared_ptr<T> addComponent();

	template <class T>
	std::shared_ptr<T> getComponent();

	template <class T>
	std::vector<std::shared_ptr<T>> getComponents();

	template <class T>
	bool hasComponent();

	void removeComponent(std::shared_ptr<Component> component);

	void update(float deltaTime);
	void render(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder);

	void renderDrawing(sre::RenderPass* renderPass);

	const glm::vec2& getPosition() const;
	void setPosition(const glm::vec2& position);

	// Sets up the gameobject for removal in next update.
	void removeObject();
	bool shouldRemoveObject();

	const std::vector<std::shared_ptr<Component>>& getComponents();

	std::string getName();
	void setName(std::string name);
	bool positionIsSet();
	static int amount;

private:
	std::string name = "_";
	bool deleteObject = false;
	glm::vec2 position = glm::vec2(0);
	bool positionSet = false;
	std::vector<std::shared_ptr<Component>> components;

	friend class PlayerController;
};

template<class T>
std::shared_ptr<T> GameObject::addComponent()
{
	auto obj = std::shared_ptr<T>(new T(this));
	components.push_back(obj);

	return obj;
}

template<class T>
std::shared_ptr<T> GameObject::getComponent()
{
	for (auto comp : components)
	{
		std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(comp);

		if (result != nullptr)
		{
			return result;
		}
	}

	return std::shared_ptr<T>();
}

template<class T>
std::vector<std::shared_ptr<T>> GameObject::getComponents()
{
	std::vector<std::shared_ptr<T>> comps;
	for (auto comp : components)
	{
		std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(comp);

		if (result != nullptr)
		{
			comps.push_back(result);
		}
	}
	return comps;
}

template<class T>
bool GameObject::hasComponent()
{
	for (auto comp : components)
	{
		std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(comp);

		if (result != nullptr)
		{
			return true;
		}
	}

	return false;
}