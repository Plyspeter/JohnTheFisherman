// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include <SDL_Mixer.h>
#include <string>

class SoundEngine {
public:
    explicit SoundEngine(int frequency, int channels, int chunksize);
    SoundEngine();
    ~SoundEngine();

    void setMusic(std::string path);
    void playMusic();
    bool isPlaying();
    bool isPaused();
    void stopMusic();
    void pauseMusic();
    void resumeMusic();

private:
    Mix_Music* music = nullptr;
    std::string path = "";
};
