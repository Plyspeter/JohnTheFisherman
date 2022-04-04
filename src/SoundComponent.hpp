// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "Component.hpp"
#include <SDL_Mixer.h>

class SoundComponent : public Component {
public:
    explicit SoundComponent(GameObject* gameObject);
    ~SoundComponent();

    void addSound(std::string name, std::string path);
    void play(std::string name, Uint8 volume);
    void stop();
    void setLoop(bool flag);
    void read(rapidjson::Value& component) override;
    void write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a) override;

private:
    bool loop = false;
    std::map<std::string, std::pair<Mix_Chunk*, std::string>> sounds;
    int channel = -1;
};
