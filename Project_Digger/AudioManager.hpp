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

	// Clears out the manager's memory
	static void release();

	// Plays a larger audio file
	void playMusic(const char* file, int loops = -1);

	// Pauses the playing music
	void pauseMusic();

	// Resumes playing the music
	void resumeMusic();

	// Plays a short audio file
	void playSoundEffect(const char* file, int loops = 0, int channel = 0);

private:

	AudioManager();
	Mix_Music* musicAudio(const char* file);
	Mix_Chunk* soundEffect(const char* file);

private:

	MusicPoolType music;
	EffectPoolType effects;
	static AudioManager* instance;

};
