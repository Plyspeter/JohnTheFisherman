// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "Component.hpp"
#include "Hookable.hpp"
#include "CantEscapeRoom.hpp"

class Fish : public Component, public Hookable, public CantEscapeRoom
{
public:
	explicit Fish(GameObject* gameObject);
	void update(float deltaTime) override;
	void read(rapidjson::Value& component) override;
	void write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a) override;
};