// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "Component.hpp"
#include "Hookable.hpp"
#include "CantEscapeRoom.hpp"

class Barrel : public Component, public Hookable, public CantEscapeRoom
{
public:
	explicit Barrel(GameObject* gameObject);
	void update(float deltaTime) override;
	void read(rapidjson::Value& component) override;
	void write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a) override;
};