#include <iostream>
#include "AudioManager.hpp"


AudioManager* AudioManager::instance = nullptr;


AudioManager* AudioManager::i() {
	return (instance ? instance : instance = new AudioManager());
}


AudioManager::AudioManager() {}


AudioManager::~AudioManager() {}


void AudioManager::release() {
	delete instance;
	instance = nullptr;
}


Mix_Music* AudioManager::musicAudio(const char* file) {

	Mix_Music* result = nullptr;
	MusicPoolType::iterator it = music.find(file);
	if (it == music.end()) {
		result = Mix_LoadMUS(file);
		music.insert(std::make_pair(file, result));
	}
	else {
		result = it->second;
	}

	if (!result)
		std::cerr << "[ERROR] Unable to load \"" << file << "\"! Error: " << Mix_GetError() << std::endl;

	return result;

}


Mix_Chunk* AudioManager::soundEffect(const char* file) {

	Mix_Chunk* result = nullptr;
	EffectPoolType::iterator it = effects.find(file);
	if (it == effects.end()) {
		result = Mix_LoadWAV(file);
		effects.insert(std::make_pair(file, result));
	} else {
		result = it->second;
	}

	if (!result)
		std::cerr << "[ERROR] Unable to load \"" << file << "\"! Error: " << Mix_GetError() << std::endl;

	return result;

}


void AudioManager::playMusic(const char* file, int loops) {
	Mix_PlayMusic(musicAudio(file), loops);
}


void AudioManager::pauseMusic() {
	if (Mix_PlayingMusic())
		Mix_PauseMusic();
}


void AudioManager::resumeMusic() {
	if (Mix_PausedMusic())
		Mix_ResumeMusic();
}


void AudioManager::playSoundEffect(const char* file, int loops, int channel) {
	Mix_PlayChannel(channel, soundEffect(file), loops);
}

