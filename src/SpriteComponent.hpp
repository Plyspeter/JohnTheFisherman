// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "Component.hpp"
#include "sre/Sprite.hpp"

class SpriteComponent : public Component {
public:
    explicit SpriteComponent(GameObject* gameObject);

    void renderSprite(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder) override;

    void setFlip(bool flip);
    bool getFlip();

    void setSprite(std::string sprite, glm::vec2 scale = defaultScale, glm::vec2 position = defaultPosition);
    std::string getSprite();

    void read(rapidjson::Value& component) override;
    void write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a) override;
private:
    std::string spriteName;
    sre::Sprite sprite;
    glm::vec2 relativePosition;
    bool flip = false;
    static constexpr glm::vec2 defaultScale = glm::vec2(1.5, 1.5);
    static constexpr glm::vec2 defaultPosition = glm::vec2(0, 0);
};

