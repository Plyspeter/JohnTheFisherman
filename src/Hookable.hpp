// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "GameObject.hpp"

class Hookable
{
public:
	explicit Hookable(GameObject* go);
	bool isHooked();
	bool isReturning();
	virtual void returnWithHook();
	virtual void hook();
	virtual void unHook();

protected:
	bool hooked = false;
	bool returning = false;

private:
	GameObject* hookableGameObject;
};