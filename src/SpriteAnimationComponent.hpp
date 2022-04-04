// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "Component.hpp"
#include "sre/Sprite.hpp"
#include "SpriteComponent.hpp"

class SpriteAnimationComponent : public Component {
public:
    SpriteAnimationComponent(GameObject* gameObject);

    void setSprites(std::string animationName, std::vector<std::string> sprites);

    void update(float deltaTime) override;

    float getAnimationTime(std::string animationName);
    std::string getCurrentAnimation();
    void setAnimationTime(std::string animationName, float animationTime);
    void setAnimation(std::string animationName);
    void setAnimationFlipOffsetMap(std::vector<std::pair<std::string, glm::vec2>> flipOffsets);
    glm::vec2 getAnimationFlipOffset(std::string name);
    void flipSprites(bool flip);

    void setRelativePosition(glm::vec2 relPos);
    
    void read(rapidjson::Value& component) override;
    void write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a) override;
private:
    std::map<std::string, float> animationTimeMap;
    std::map<std::string, std::vector<std::string>> animationSpriteMap;
    std::map<std::string, glm::vec2> animationFlipOffsetMap; //only for spawn engine
    std::string currentAnimation;
    float time = 0;
    int spriteIndex = 0;
    bool flip = false;
    glm::vec2 relativePosition = { 0, 0 };
    glm::vec2 scale = { 1.5, 1.5 };
    glm::vec2 flipOffset = { 0, 0 };
};


