// Game Programming 2020
// Exam Project: John the Fisherman

#include "SoundEngine.hpp"
#include <iostream>

SoundEngine::SoundEngine(int frequency, int channels, int chunksize)
{
	Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, channels, chunksize);
}

SoundEngine::SoundEngine()
{
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
}

SoundEngine::~SoundEngine()
{
	Mix_FreeMusic(music);
	Mix_CloseAudio();
}

void SoundEngine::setMusic(std::string name)
{
	this->path = "./resources/music/" + name;
	Mix_FreeMusic(music);
	music = Mix_LoadMUS(path.c_str());
	Mix_VolumeMusic(100);
	if (music == nullptr) std::cout << "setMusic error: " << Mix_GetError() << std::endl;
}

void SoundEngine::playMusic()
{
	Mix_PlayMusic(music, -1);
}

bool SoundEngine::isPlaying()
{
	return Mix_PlayingMusic();
}

bool SoundEngine::isPaused()
{
	return Mix_PausedMusic();
}

void SoundEngine::stopMusic()
{
	Mix_HaltMusic();
}

void SoundEngine::pauseMusic()
{
	if(isPlaying() && !isPaused()) Mix_PauseMusic();
}

void SoundEngine::resumeMusic()
{
	if (isPaused()) Mix_ResumeMusic();
}
