// Game Programming 2020
// Exam Project: John the Fisherman

#include "SpriteComponent.hpp"
#include "GameObject.hpp"
#include "FishermanGame.hpp"
#include "Room.hpp"
#include "SpriteAnimationComponent.hpp"

SpriteComponent::SpriteComponent(GameObject* gameObject) : Component(gameObject) {}

void SpriteComponent::renderSprite(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder) {
    auto anim = gameObject->getComponent<SpriteAnimationComponent>();
    auto absolutePosition = gameObject->getPosition() + relativePosition;
    if (flip) {
        if (anim != nullptr) {
            absolutePosition += anim->getAnimationFlipOffset(this->spriteName);
        }
        sprite.setFlip(glm::vec2(true, false));
    }
    else {
        sprite.setFlip(glm::vec2(false, false));
    }
    sprite.setPosition(absolutePosition);
    spriteBatchBuilder.addSprite(sprite);
}

void SpriteComponent::setFlip(bool flip)
{
    this->flip = flip;
}

bool SpriteComponent::getFlip()
{
    return flip;
}

void SpriteComponent::setSprite(std::string sprite, glm::vec2 scale, glm::vec2 relativePosition) {
    spriteName = sprite;
    this->sprite = FishermanGame::instance->getSpriteAtlas()->get(sprite);
    this->sprite.setScale(scale);
    this->relativePosition = relativePosition;
}

std::string SpriteComponent::getSprite() {
    return spriteName;
}

void SpriteComponent::read(rapidjson::Value& component)
{
    flip = component["flip"].GetBool();
    auto scale = defaultScale;
    relativePosition = defaultPosition;
    if (component.HasMember("scale")) scale = { component["scale"]["x"].GetFloat(), component["scale"]["y"].GetFloat() };
    if (component.HasMember("relativePosition")) relativePosition = { component["relativePosition"]["x"].GetFloat(), component["relativePosition"]["y"].GetFloat() };
    setSprite(component["sprite"].GetString(),scale ,relativePosition);
}

void SpriteComponent::write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a)
{
    rapidjson::Value componenType((int)Comp::SpriteC);
    c.AddMember("componentType", componenType, a);

    rapidjson::Value flip(flip);
    c.AddMember("flip", flip, a);
    rapidjson::Value sprite(spriteName.c_str(), spriteName.length(), a);
    c.AddMember("sprite", sprite, a);

    rapidjson::Value relPos(rapidjson::kObjectType);
    {
        rapidjson::Value x(relativePosition.x);
        relPos.AddMember("x", x, a);
        rapidjson::Value y(relativePosition.y);
        relPos.AddMember("y", y, a);
    }
    c.AddMember("relativePosition", relPos, a);

    rapidjson::Value jScale(rapidjson::kObjectType);
    {
        rapidjson::Value x(this->sprite.getScale().x);
        jScale.AddMember("x", x, a);
        rapidjson::Value y(this->sprite.getScale().y);
        jScale.AddMember("y", y, a);
    }
    c.AddMember("scale", jScale, a);
}
