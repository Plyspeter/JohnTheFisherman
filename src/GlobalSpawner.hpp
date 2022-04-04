// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "GameObject.hpp"
class GlobalSpawner
{
public:
	static std::shared_ptr<GameObject> spawnPlayer();
};