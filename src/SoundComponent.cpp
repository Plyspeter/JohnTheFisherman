// Game Programming 2020
// Exam Project: John the Fisherman

#include "SoundComponent.hpp"
#include "Room.hpp"
#include <iostream>
#include "GameObject.hpp"

SoundComponent::SoundComponent(GameObject* gameObject) : Component(gameObject) {}

SoundComponent::~SoundComponent()
{
	for each (auto pair in sounds)
	{
		Mix_FreeChunk(pair.second.first);
	}
}

void SoundComponent::addSound(std::string name, std::string fileName)
{
	auto path = "./resources/soundEffects/" + fileName;
	Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
	if (sound == nullptr)
	{
		std::cout << "setSound error in GameObject " << gameObject->getName() << ": " << Mix_GetError() << std::endl;
		std::cout << "path: " << path << std::endl;
	}
	sounds.insert({ name, { sound, fileName } });
}

void SoundComponent::play(std::string name, Uint8 volume)
{
	Mix_Chunk* sound = sounds[name].first;
	if (sound == nullptr) std::cout << "tried to play null sound in GameObject " << gameObject->getName() << ": " << std::endl;
 	sound->volume = volume;
	channel = Mix_PlayChannel(-1, sound, loop ? -1 : 0);
}

void SoundComponent::stop()
{
	if (channel > -1 && Mix_Playing(channel))
	{
		Mix_HaltChannel(channel);
	}
	channel = -1;
}

void SoundComponent::setLoop(bool flag)
{
	loop = flag;
}

void SoundComponent::read(rapidjson::Value& component)
{
	loop = component["loop"].GetBool();
	for each (rapidjson::Value& pair in component["sounds"].GetArray())
	{
		addSound(pair["name"].GetString(), pair["sound"].GetString());
	}
}

void SoundComponent::write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a)
{
	rapidjson::Value componenType((int)Comp::SoundC);
	c.AddMember("componentType", componenType, a);
	rapidjson::Value loop(loop);
	c.AddMember("loop", loop, a);
	rapidjson::Value jSounds(rapidjson::kArrayType);
	for each (auto pair in sounds)
	{
		rapidjson::Value p(rapidjson::kObjectType);
		rapidjson::Value name(pair.first.c_str(), pair.first.length(), a);
		p.AddMember("name", name, a);
		rapidjson::Value sound(pair.second.second.c_str(), pair.second.second.length(), a);
		p.AddMember("sound", sound, a);
		jSounds.PushBack(p, a);
	}
	c.AddMember("sounds", jSounds, a);
}
