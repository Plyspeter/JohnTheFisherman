// Game Programming 2020
// Exam Project: John the Fisherman

#include "SpriteAnimationComponent.hpp"
#include "GameObject.hpp"
#include "Room.hpp"
#include "FishermanGame.hpp"

SpriteAnimationComponent::SpriteAnimationComponent(GameObject* gameObject) : Component(gameObject) {}

void SpriteAnimationComponent::setSprites(std::string animationName, std::vector<std::string> sprites)
{
    animationSpriteMap.insert(std::pair<std::string, std::vector<std::string>>(animationName, sprites));
}

void SpriteAnimationComponent::update(float deltaTime) {
    auto spriteComponent = gameObject->getComponent<SpriteComponent>();
    
    assert(spriteComponent != nullptr);

    time += deltaTime;

    auto currTime = getAnimationTime(currentAnimation);
    auto currSprites = animationSpriteMap.find(currentAnimation)->second;
    if (time == deltaTime) { //Hard swap between animations with no transition
        spriteComponent->setSprite(currSprites[0], scale, relativePosition);
    }
    if (time > currTime) {
        time = fmod(time, currTime);
        spriteIndex = (spriteIndex + 1) % currSprites.size();
        spriteComponent->setSprite(currSprites[spriteIndex], scale, relativePosition);
    }
}

float SpriteAnimationComponent::getAnimationTime(std::string animationName)
{
    return animationTimeMap.find(animationName)->second;
}

std::string SpriteAnimationComponent::getCurrentAnimation()
{
    return currentAnimation;
}

void SpriteAnimationComponent::setAnimationTime(std::string animationName, float animationTime)
{
    animationTimeMap.insert(std::pair<std::string, float>(animationName, animationTime));
}

void SpriteAnimationComponent::setAnimation(std::string animationName)
{
    currentAnimation = animationName;
    time = 0;
    spriteIndex = 0;
    animationFlipOffsetMap[animationName] = { 0, 0};
}

void SpriteAnimationComponent::setAnimationFlipOffsetMap(std::vector<std::pair<std::string, glm::vec2>> flipOffsets)
{
    for each (auto pair in flipOffsets)
    {
        animationFlipOffsetMap.insert(pair);
    }
}

glm::vec2 SpriteAnimationComponent::getAnimationFlipOffset(std::string name) {
    return animationFlipOffsetMap[name] + flipOffset;
}

void SpriteAnimationComponent::flipSprites(bool flip)
{
    auto spriteComponent = gameObject->getComponent<SpriteComponent>();
    spriteComponent->setFlip(flip);
    this->flip = flip;
}

void SpriteAnimationComponent::setRelativePosition(glm::vec2 relPos)
{
    relativePosition = relPos;
}



void SpriteAnimationComponent::read(rapidjson::Value& component)
{
    for each (rapidjson::Value& pair in component["animationSpriteMap"].GetArray())
    {
        std::string name = pair["name"].GetString();
        std::vector<std::string> sprites;
        for each (rapidjson::Value& sprite in pair["sprites"].GetArray())
        {
            sprites.push_back(sprite.GetString());
        }
        animationSpriteMap.insert(std::pair<std::string, std::vector<std::string>>(name, sprites));
    }

    for each (rapidjson::Value& pair in component["animationTimeMap"].GetArray())
    {
        std::string name = pair["name"].GetString();
        float time = pair["time"].GetFloat();
        animationTimeMap.insert(std::pair<std::string, float>(name, time));
    }

    if (component.HasMember("relativePosition"))
        setRelativePosition({ component["relativePosition"]["x"].GetFloat(), component["relativePosition"]["y"].GetFloat() });

    if (component.HasMember("scale"))
        scale = { component["scale"]["x"].GetFloat(), component["scale"]["y"].GetFloat() };

    if (component.HasMember("flipOffset"))
        flipOffset = { component["flipOffset"]["x"].GetFloat(), component["flipOffset"]["y"].GetFloat() };
    
    currentAnimation = component["currentAnimation"].GetString();
    time = component["time"].GetFloat();
    spriteIndex = component["spriteIndex"].GetInt();
    flip = component["flip"].GetBool();
}

void SpriteAnimationComponent::write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a)
{
    rapidjson::Value componenType((int)Comp::AnimationC);
    c.AddMember("componentType", componenType, a);

    rapidjson::Value jAnimationTimeMap(rapidjson::kArrayType);
    for each (auto pair in animationTimeMap)
    {
        rapidjson::Value jPair(rapidjson::kObjectType);
        rapidjson::Value name(pair.first.c_str(), pair.first.length(), a);
        jPair.AddMember("name", name, a);
        rapidjson::Value time(pair.second);
        jPair.AddMember("time", time, a);
        jAnimationTimeMap.PushBack(jPair, a);
    }
    c.AddMember("animationTimeMap", jAnimationTimeMap, a);

    rapidjson::Value jAnimationSpriteMap(rapidjson::kArrayType);
    for each (auto pair in animationSpriteMap) 
    {
        rapidjson::Value jPair(rapidjson::kObjectType);
        rapidjson::Value name(pair.first.c_str(), pair.first.length(), a);
        jPair.AddMember("name", name, a);

        rapidjson::Value jOffset(rapidjson::kObjectType);
        {
            rapidjson::Value x(flipOffset.x);
            jOffset.AddMember("x", x, a);
            rapidjson::Value y(flipOffset.y);
            jOffset.AddMember("y", y, a);
        }
        jPair.AddMember("flipOffset", jOffset, a);

        rapidjson::Value sprites(rapidjson::kArrayType);
        for each (auto sprite in pair.second)
        {
            rapidjson::Value sprite(sprite.c_str(), sprite.length(), a);
            sprites.PushBack(sprite, a);
        }
        jPair.AddMember("sprites", sprites, a);

        jAnimationSpriteMap.PushBack(jPair, a);
    }
    c.AddMember("animationSpriteMap", jAnimationSpriteMap, a);
    
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
        rapidjson::Value x(scale.x);
        jScale.AddMember("x", x, a);
        rapidjson::Value y(scale.y);
        jScale.AddMember("y", y, a);
    }
    c.AddMember("scale", jScale, a);

    rapidjson::Value currentAnimation(currentAnimation.c_str(), currentAnimation.length(), a);
    c.AddMember("currentAnimation", currentAnimation, a);
    rapidjson::Value time(time);
    c.AddMember("time", time, a);
    rapidjson::Value spriteIndex(spriteIndex);
    c.AddMember("spriteIndex", spriteIndex, a);
    rapidjson::Value flip(flip);
    c.AddMember("flip", flip, a);
}

