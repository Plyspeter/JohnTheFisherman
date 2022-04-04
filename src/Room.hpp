// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "Component.hpp"
#include "rapidjson/document.h"
#include "Camera.hpp"

class GameObject;

enum class Comp {
	PhysicsC = 0,
	EnemyC = 1,
	SkeletonRunnerC = 2,
	AnimationC = 3,
	SpriteC = 4,
	SoundC = 5,
	MovingPlatformC = 6,
	EnemyFlyerC = 7,
	BarrelC = 8,
	FishC = 9,
};

class Room
{
public:
	explicit Room(int id);
	std::string getBackground();
	std::vector<std::shared_ptr<GameObject>> read(glm::vec2 origin = { 0, 0 });
	void write(std::vector<std::shared_ptr<GameObject>>);
	int getLink(Direction d);

private:
	void clearGameObjects();
	std::string getCopyPath(std::string path);
	void readComponents(rapidjson::Value& jComponents, std::shared_ptr<GameObject> go);
	void readPrefab(std::shared_ptr<GameObject> go);

	std::vector<std::shared_ptr<GameObject>> gameObjects;
	std::string path = "";
	const std::string roomFolder = "./resources/rooms/";
	const std::string prefabFolder = "./resources/prefabs/";
	const std::string extension = ".json";
	bool hasRead = false;
	int id;
	std::string backgroundPath;
	int top;
	int bottom;
	int left;
	int right;
};