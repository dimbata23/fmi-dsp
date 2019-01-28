#pragma once

#ifdef _MSC_VER
#include <SDL_mixer.h>
#else
#include <SDL2/SDL_mixer.h>
#endif
#include <unordered_map>

using MusicPoolType = std::unordered_map<const char*, Mix_Music*>;
using EffectPoolType = std::unordered_map<const char*, Mix_Chunk*>;

class AudioManager {

public:
	
	// Get the AudioManager instance
	static AudioManager* i();
	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;
	~AudioManager();

	static void release();
	Mix_Music* musicAudio(const char* file);
	Mix_Chunk* soundEffect(const char* file);

	void playMusic(Mix_Music* music, int loops = -1);
	void pauseMusic();
	void resumeMusic();

	void playSoundEffect(Mix_Chunk* soundEffect, int loops = 0, int channel = 0);

private:

	AudioManager();

private:

	MusicPoolType music;
	EffectPoolType effects;
	static AudioManager* instance;

};
