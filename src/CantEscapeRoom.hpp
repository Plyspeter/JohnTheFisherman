// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "GameObject.hpp"

class CantEscapeRoom
{
public:
	explicit CantEscapeRoom(GameObject* gameObject);
	void update(float deltaTime);

private:
	GameObject* cesGameObject;
};
