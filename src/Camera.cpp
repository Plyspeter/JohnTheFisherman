// Game Programming 2020
// Exam Project: John the Fisherman
#include "Camera.hpp"
#include "FishermanGame.hpp"

using namespace glm;

Camera::Camera()
{
    camera.setOrthographicProjection(FishermanGame::windowSize.y / 2, -1, 1);
    offset = { 0, FishermanGame::windowSize.y / 2 };
}

sre::Camera& Camera::getCamera() {
    return camera;
}

glm::vec2 Camera::getCurrentScreenCenter()
{
    return currScreenCenter;
}

glm::vec2 Camera::getOffset()
{
    return offset;
}

glm::vec2 Camera::shift(Direction d)
{
    switch (d)
    {
    case Direction::Up:
        nextScreenCenter = { currScreenCenter.x, currScreenCenter.y + FishermanGame::windowSize.y / 2 - shiftOffset.y };
        break;
    case Direction::Down:
        nextScreenCenter = { currScreenCenter.x, currScreenCenter.y - FishermanGame::windowSize.y / 2 + shiftOffset.y };
        break;
    case Direction::Left:
        nextScreenCenter = { currScreenCenter.x - FishermanGame::windowSize.x + shiftOffset.x, currScreenCenter.y };
        break;
    case Direction::Right:
    nextScreenCenter = { currScreenCenter.x + FishermanGame::windowSize.x - shiftOffset.x, currScreenCenter.y };
        break;
    default:
        break;
    }
    if (FishermanGame::instance->getCurrentState() == GameState::Running)
        FishermanGame::instance->setCurrentState(GameState::CameraShift);
    timer = 0;
    return nextScreenCenter;
}

void Camera::update(float deltaTime) {
    bool shifting = FishermanGame::instance->getCurrentState() == GameState::CameraShift;
    if (shifting) {
        timer += deltaTime;
    }

    float t = timer / shiftTime;
    if (t >= 1) {
        if (shifting)
            FishermanGame::instance->setCurrentState(GameState::Running);
        timer = 0;
        currScreenCenter = nextScreenCenter;
    }

    vec2 position = glm::mix(currScreenCenter, nextScreenCenter, t);

    position.x += offset.x;
    position.y = offset.y;

    vec3 eye(position, 0);
    vec3 at(position, -1);
    vec3 up(0, 1, 0);
    camera.lookAt(eye, at, up);
}
