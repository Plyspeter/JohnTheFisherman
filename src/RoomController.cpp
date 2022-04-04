// Game Programming 2020
// Exam Project: John the Fisherman

#include "RoomController.hpp"
#include "PlayerController.hpp"
#include "Room.hpp"
#include "FishermanGame.hpp"

RoomController::RoomController(std::shared_ptr<Camera> camera, std::shared_ptr<GameObject> playerObject)
{
    currentRoomId = 0;
    this->camera = camera;
    player = playerObject;
    auto firstRoom = std::make_shared<Room>(currentRoomId);
    rooms[currentRoomId] = firstRoom;
    FishermanGame::instance->addRoomGameObjects(firstRoom->read());
}

void markForDelete(std::vector<std::shared_ptr<GameObject>> gos)
{
    if (gos.size() > 0)
    {
        FishermanGame::instance->clearGlobalGameObjectsExceptPlayer();
        FishermanGame::instance->clearRoomGameObjects();
        FishermanGame::instance->addRoomGameObjects(gos);
    }
}

std::function<void()> RoomController::update()
{
    std::vector<std::shared_ptr<GameObject>> gos;
    auto playerPos = player->getPosition();
    auto currScreenCenter = camera->getCurrentScreenCenter();
    auto offset = camera->getOffset();
    if (FishermanGame::instance->getCurrentState() == GameState::Running) {
        if      (playerPos.x > currScreenCenter.x + FishermanGame::windowSize.x / 2 + offset.x) gos = loadRoom(Direction::Right);
        else if (playerPos.x < currScreenCenter.x - FishermanGame::windowSize.x / 2 + offset.x) gos = loadRoom(Direction::Left);
        else if (playerPos.y < currScreenCenter.y - FishermanGame::windowSize.y / 2 + offset.y) gos = loadRoom(Direction::Down);
        else if (playerPos.y > currScreenCenter.y + FishermanGame::windowSize.y / 2 + offset.y) gos = loadRoom(Direction::Up);
    }
    return std::bind(markForDelete, gos);
}

std::vector<std::shared_ptr<GameObject>> RoomController::loadRoom(Direction d)
{
    std::vector<std::shared_ptr<GameObject>> gos;
    auto newId = rooms[currentRoomId]->getLink(d);
    if (newId == -1) return gos;
    auto nextScreenCenter = camera->shift(d);
    rooms[currentRoomId]->write(FishermanGame::instance->getRoomGameObjects());
    auto newRoom = rooms[newId];
    if (newRoom.get() == nullptr) {
        newRoom = std::make_shared<Room>(newId);
        rooms[newId] = newRoom;
    }
    currentRoomId = newId;
    return newRoom->read(nextScreenCenter);
    
}
