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

    TextureManager();
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    ~TextureManager();

    SDL_Texture* sprite(const char* file, SDL_Renderer* ren);

private:

    static SDL_Texture* loadTexture(const char* fileName, SDL_Renderer* ren);

private:

    TexturesPoolType textures;

};
