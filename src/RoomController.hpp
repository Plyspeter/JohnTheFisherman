// Game Programming 2020
// Exam Project: John the Fisherman
#pragma once
#include "Camera.hpp"

class RoomController
{
public:
	explicit RoomController(std::shared_ptr<Camera> camera, std::shared_ptr<GameObject> playerObject);
	std::function<void()> update();
	std::vector<std::shared_ptr<GameObject>> loadRoom(Direction d);

private:
	std::shared_ptr<GameObject> player;
	std::shared_ptr<Camera> camera;
	std::map<int, std::shared_ptr<Room>> rooms;
	int currentRoomId;
};