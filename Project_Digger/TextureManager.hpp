#pragma once

#ifdef _MSC_VER
#include <SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif
#include <unordered_map>

using TexturesPoolType = std::unordered_map<const char*, SDL_Texture*>;

class TextureManager {

public:

    // Get the TextureManager instance
	static TextureManager* i();
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    ~TextureManager();

    static void release();
    SDL_Texture* sprite(const char* file, SDL_Renderer* ren);

private:

    TextureManager();
    static SDL_Texture* loadTexture(const char* fileName, SDL_Renderer* ren);

private:

    TexturesPoolType textures;
    static TextureManager* instance;

};
