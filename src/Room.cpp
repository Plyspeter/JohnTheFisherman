// Game Programming 2020
// Exam Project: John the Fisherman

#include "Room.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"
#include <fstream>
#include <iostream>
#include "PlayerController.hpp"
#include "PhysicsComponent.hpp"
#include "MovingPlatform.hpp"
#include "SpriteAnimationComponent.hpp"
#include "SpriteComponent.hpp"
#include "SoundComponent.hpp"
#include "EnemyRunner.hpp"
#include "EnemyFlyer.hpp"
#include "Barrel.hpp"
#include "Fish.hpp"

using namespace std;
using namespace rapidjson;

Room::Room(int id) 
{
    this->path = roomFolder + to_string(id) + extension;
}

string Room::getBackground() {return backgroundPath;}

void Room::clearGameObjects() { gameObjects.clear(); }

std::string Room::getCopyPath(std::string path)
{
    int extensionIndex = path.find_last_of('.');
    string extension = path.substr(extensionIndex);
    string p = path.substr(0, path.length() - extension.length());
    return p + "copy" + extension;
}

int Room::getLink(Direction d) 
{
    switch (d)
    {
    case Direction::Up:
        return top;
        break;
    case Direction::Down:
        return bottom;
        break;
    case Direction::Left:
        return left;
        break;
    case Direction::Right:
        return right;
        break;
    default:
        return -1;
        break;
    }
}

std::vector<shared_ptr<GameObject>> Room::read(glm::vec2 origin)
{
    std::string actualPath = path;
    if (hasRead) 
    {
        origin = glm::vec2(0);
        actualPath = getCopyPath(path);
    }
    ifstream fis(actualPath);
    if (fis.fail()) throw "File does not exist";
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream<kParseCommentsFlag>(isw);

    id = d["id"].GetInt();
    backgroundPath = d["background"].GetString();
    auto links = d["links"].GetObject();
    top = links["top"].GetInt();
    bottom = links["bottom"].GetInt();
    left = links["left"].GetInt();
    right = links["right"].GetInt();

    if (d.HasMember("prefabs"))
    {
        for each (auto & obj in d["prefabs"].GetArray())
        {
            auto go = std::shared_ptr<GameObject>(new GameObject());
            go->setName(obj["name"].GetString());
            go->setPosition({ obj["position"]["x"].GetFloat() + origin.x, obj["position"]["y"].GetFloat() + origin.y });
            readPrefab(go);
            gameObjects.push_back(go);
        }
    }
    for each (auto& obj in d["gameObjects"].GetArray())
    {
        auto go = std::shared_ptr<GameObject>(new GameObject());
        go->setName(obj["name"].GetString());
        go->setPosition({ obj["position"]["x"].GetFloat() + origin.x, obj["position"]["y"].GetFloat() + origin.y });
        readComponents(obj["components"], go);
        gameObjects.push_back(go);
    }
    fis.close();
    hasRead = true;
    auto gos = gameObjects;
    gameObjects.clear();
    return gos;
}

void Room::readPrefab(std::shared_ptr<GameObject> go)
{
    ifstream fis(prefabFolder + go->getName() + ".json");
    if (fis.fail()) throw "File does not exist";
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream<kParseCommentsFlag>(isw);

    readComponents(d["components"], go);
    fis.close();
}

void Room::readComponents(Value& jComponents, std::shared_ptr<GameObject> go)
{
    for each (auto& jComponent in jComponents.GetArray())
    {
        std::shared_ptr<Component> component = std::shared_ptr<Component>();
        auto type = (Comp)jComponent["componentType"].GetInt();

        switch (type)
        {
        case Comp::PhysicsC:
            component = go->addComponent<PhysicsComponent>();
            break;
        case Comp::EnemyC:
            component = go->addComponent<Enemy>();
            break;
        case Comp::SkeletonRunnerC:
            component = go->addComponent<EnemyRunner>();
            break;
        case Comp::AnimationC:
            component = go->addComponent<SpriteAnimationComponent>();
            break;
        case Comp::SpriteC:
            component = go->addComponent<SpriteComponent>();
            break;
        case Comp::SoundC:
            component = go->addComponent<SoundComponent>();
            break;
        case Comp::MovingPlatformC:
            component = go->addComponent<MovingPlatform>();
            break;
        case Comp::EnemyFlyerC:
            component = go->addComponent<EnemyFlyer>();
            break;
        case Comp::BarrelC:
            component = go->addComponent<Barrel>();
            break;
        case Comp::FishC:
            component = go->addComponent<Fish>();
            break;
        default:
            break;
        }
        if (component.get() != nullptr) component->read(jComponent);
        else std::cout << "Component not recognized";
    }
}

void Room::write(std::vector<std::shared_ptr<GameObject>> gos)
{
    gameObjects = gos;
    ofstream fos(getCopyPath(path));
    if (fos.fail()) throw "File does not exist";
    OStreamWrapper osw(fos);
    Writer<OStreamWrapper> writer(osw);
    Document d(kObjectType);
    Document::AllocatorType& a = d.GetAllocator();

    Value id(id);
    d.AddMember("id", id, a);
    Value background(backgroundPath.c_str(), backgroundPath.length(), a);
    d.AddMember("background", background, a);

    Value links(kObjectType);
    {
        Value top(top);
        links.AddMember("top", top, a);
        Value bottom(bottom);
        links.AddMember("bottom", bottom, a);
        Value left(left);
        links.AddMember("left", left, a);
        Value right(right);
        links.AddMember("right", right, a);
    }
    d.AddMember("links", links, a);

    Value gameObjs(kArrayType);
    for each (auto go in gameObjects)
    {
        Value gameObj(kObjectType);

        Value name(go->getName().c_str(), go->getName().size(), a);
        gameObj.AddMember("name", name, a);

        Value position(kObjectType);
        {
            Value x(go->getPosition().x);
            position.AddMember("x", x, a);
            Value y(go->getPosition().y);
            position.AddMember("y", y, a);
        }
        gameObj.AddMember("position", position, a);

        Value components(kArrayType);
        for each (auto comp in go->getComponents())
        {
            Value c(kObjectType);
            comp->write(c, a);
            components.PushBack(c, a);
        }
        gameObj.AddMember("components", components, a);

        gameObjs.PushBack(gameObj, a);
    }

    d.AddMember("gameObjects", gameObjs, a);
    d.Accept(writer);
    fos.close();
    clearGameObjects();
}
