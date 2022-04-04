// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "sre/Camera.hpp"
#include "glm/glm.hpp"
#include "Component.hpp"

enum class Direction
{
    Up,
    Down,
    Left,
    Right
};

class Camera {
public:
    explicit Camera();

    void update(float deltaTime);

    sre::Camera& getCamera();
    glm::vec2 getCurrentScreenCenter();
    glm::vec2 getOffset();
    glm::vec2 shift(Direction d);
private:
    float timer = 0; //To keep track of how long a shift has been running
    float shiftTime = 2; //Sets how long a camera shift take
    sre::Camera camera;
    glm::vec2 offset;  //Offsets the camera's position
    glm::vec2 shiftOffset = {2, 2}; //Offsets how long the shift is in order for the player }o be within the shift
    glm::vec2 currScreenCenter = glm::vec2(0); //Current screen center
    glm::vec2 nextScreenCenter = currScreenCenter; //Next screen center for linear interpolation from current
};

